#pragma once

#include <QColor>
#include <QPlainTextEdit>
#include <QSharedPointer>
#include <QSyntaxHighlighter>

#include "indent/indenter.h"


namespace Qutepart {

class Qutepart: public QPlainTextEdit {
    Q_OBJECT

public:
    Qutepart(QWidget *parent = nullptr, const QString &text = QString::null);

    // Not copyable or movable
    Qutepart(const Qutepart&) = delete;
    Qutepart& operator=(const Qutepart&) = delete;
    Qutepart(Qutepart&&) = delete;
    Qutepart& operator=(Qutepart&&) = delete;

    void initHighlighter(const QString& filePath);

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void paintEvent(QPaintEvent *event);

private:
    void initActions();
    QAction* createAction(const QString& text, QKeySequence shortcut,
                          const QString& iconFileName=QString::null);

    void drawIndentMarkersAndEdge(const QRect& rect);
    void drawIndentMarker(QPainter* painter, QTextBlock block, int column);
    void drawEdgeLine(QPainter* painter, QTextBlock block, int edgePos);
    void drawWhiteSpace(QPainter* painter, QTextBlock block, int column, QChar ch);
    int effectiveEdgePos(const QString& text);
    QVector<bool> chooseVisibleWhitespace(const QString& text);

    QRect cursorRect(QTextBlock block, int column, int offset) const;

    QSharedPointer<QSyntaxHighlighter> highlighter_;
    Indenter indenter_;

    bool drawIndentations_;
    bool drawAnyWhitespace_;
    bool drawIncorrectIndentation_;
    bool drawSolidEdge_;
    int lineLengthEdge_;
    QColor lineLengthEdgeColor_;
};

}; // namespace Qutepart
