#include <Qt>
#include <QTextLayout>

#include "language.h"
#include "syntax_highlighter.h"


namespace Qutepart {

SyntaxHighlighter::SyntaxHighlighter(QTextDocument* parent, QSharedPointer<Language> language):
    QSyntaxHighlighter(parent),
    language(language)
{}

SyntaxHighlighter::SyntaxHighlighter(QObject* parent, QSharedPointer<Language> language):
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

}
