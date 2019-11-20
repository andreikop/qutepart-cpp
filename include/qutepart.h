#pragma once

#include <QColor>
#include <QPlainTextEdit>
#include <QSharedPointer>
#include <QSyntaxHighlighter>

#include "indent/indenter.h"


namespace Qutepart {


struct LangInfo {
    QString id;  // Internal unique language ID
    QStringList names;   // user readable language names
    QString indenter;   // indenter algorithm name for the language. Might be null.

    LangInfo() = default;

    inline LangInfo(const QString& id, const QStringList& names, const QString& indenter):
        id(id),
        names(names),
        indenter(indenter)
    {};

    inline bool isValid() const {
        return ! id.isEmpty();
    }
};

/* Choose language by available parameters
 * First parameters have higher priority
 * Returns QString::null if failed
 */
LangInfo chooseLanguage(
    const QString& mimeType=QString::null,
    const QString& languageName=QString::null,
    const QString& sourceFilePath=QString::null,
    const QString& firstLine=QString::null);


class Qutepart: public QPlainTextEdit {
    Q_OBJECT

public:
    Qutepart(QWidget *parent = nullptr, const QString &text = QString::null);

    // Not copyable or movable
    Qutepart(const Qutepart&) = delete;
    Qutepart& operator=(const Qutepart&) = delete;
    Qutepart(Qutepart&&) = delete;
    Qutepart& operator=(Qutepart&&) = delete;

    void setFont(const QFont&); // NOTE this method is not virtual in QWidget

    // Set highlighter. Use chooseLanguage() to get the id
    void setHighlighter(const QString& languageId);

    // Editor configuration
    bool indentUseTabs() const;
    void setIndentUseTabs(bool);

    int indentWidth() const;
    void setIndentWidth(int);

    // Editor apperance configuration
    bool drawIndentations() const;
    void setDrawIndentations(bool);

    bool drawAnyWhitespace() const;
    void setDrawAnyWhitespace(bool);

    bool drawIncorrectIndentation() const;
    void setDrawIncorrectIndentation(bool);

    bool drawSolidEdge() const;
    void setDrawSolidEdge(bool);

    int lineLengthEdge() const;
    void setLineLengthEdge(int);

    QColor lineLengthEdgeColor() const;
    void setLineLengthEdgeColor(QColor);

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void paintEvent(QPaintEvent *event);

private:
    void initActions();
    QAction* createAction(const QString& text, QKeySequence shortcut,
                          const QString& iconFileName=QString::null);

    // whitespace and edge drawing
    void drawIndentMarkersAndEdge(const QRect& rect);
    void drawIndentMarker(QPainter* painter, QTextBlock block, int column);
    void drawEdgeLine(QPainter* painter, QTextBlock block, int edgePos);
    void drawWhiteSpace(QPainter* painter, QTextBlock block, int column, QChar ch);
    int effectiveEdgePos(const QString& text);
    void chooseVisibleWhitespace(const QString& text, QVector<bool>* result);
    void setSolidEdgeGeometry();
    void updateViewport();
    void updateViewportMargins();
    void resizeEvent(QResizeEvent* event) override;

    void updateTabStopWidth();

    QRect cursorRect(QTextBlock block, int column, int offset) const;

    QSharedPointer<QSyntaxHighlighter> highlighter_;
    Indenter indenter_;

    bool drawIndentations_;
    bool drawAnyWhitespace_;
    bool drawIncorrectIndentation_;
    bool drawSolidEdge_;
    int lineLengthEdge_;
    QColor lineLengthEdgeColor_;
    QWidget* solidEdgeLine_;
    int totalMarginWidth_;
};

}; // namespace Qutepart
