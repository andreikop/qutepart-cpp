#pragma once

#include <memory>

#include <QColor>
#include <QPlainTextEdit>
#include <QSharedPointer>
#include <QSyntaxHighlighter>


namespace Qutepart {


enum IndentAlg {
    INDENT_ALG_NONE = 0,
    INDENT_ALG_NORMAL,
    INDENT_ALG_LISP
};


struct LangInfo {
    QString id;  // Internal unique language ID
    QStringList names;   // user readable language names
    IndentAlg indentAlg;   // indenter algorithm for the language

    LangInfo() = default;

    inline LangInfo(const QString& id, const QStringList& names, IndentAlg indentAlg):
        id(id),
        names(names),
        indentAlg(indentAlg)
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

class Indenter;

class Qutepart: public QPlainTextEdit {
    Q_OBJECT

public:
    Qutepart(QWidget *parent = nullptr, const QString &text = QString::null);

    // Not copyable or movable
    Qutepart(const Qutepart&) = delete;
    Qutepart& operator=(const Qutepart&) = delete;
    Qutepart(Qutepart&&) = delete;
    Qutepart& operator=(Qutepart&&) = delete;

    virtual ~Qutepart();

    void setFont(const QFont&); // NOTE this method is not virtual in QWidget

    // Set highlighter. Use chooseLanguage() to get the id
    void setHighlighter(const QString& languageId);

    // Set indenter algorithm. Use chooseLanguage() to choose algorithm
    void setIndentAlgorithm(IndentAlg indentAlg);

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
    std::unique_ptr<Indenter> indenter_;

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
