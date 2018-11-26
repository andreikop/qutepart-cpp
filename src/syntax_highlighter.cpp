#include <Qt>
#include <QDebug>

#include "syntax_highlighter.h"


SyntaxHighlighter::SyntaxHighlighter(QTextDocument* parent)
    : QSyntaxHighlighter(parent)
{}

void SyntaxHighlighter::highlightBlock(const QString &text) {
    setFormat(8, 4, Qt::red);
}
