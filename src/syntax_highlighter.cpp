#include <Qt>
#include <QDebug>
#include <QTextLayout>

#include "language.h"

#include "syntax_highlighter.h"


SyntaxHighlighter::SyntaxHighlighter(QTextDocument* parent, Language* language):
    QSyntaxHighlighter(parent),
    language(language)
{}

void SyntaxHighlighter::highlightBlock(const QString&) {
    QVector<QTextLayout::FormatRange> formats;

    language->highlightBlock(currentBlock(), formats);

    foreach(QTextLayout::FormatRange range, formats) {
        setFormat(range.start, range.length, range.format);
    }
}
