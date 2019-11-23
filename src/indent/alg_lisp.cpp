#include <QDebug>

#include "text_block_utils.h"

#include "alg_lisp.h"


namespace Qutepart {

const QString& IndentAlgLisp::triggerCharacters() const {
    static QString ch(';');
    return ch;
}

QString IndentAlgLisp::computeSmartIndent(
        QTextBlock block,
        const QString& configuredIndent,
        QChar typedKey) const
{
    /* special rules: ;;; -> indent 0
                      ;;  -> align with next line, if possible
                      ;   -> usually on the same line as code -> ignore
     */
    QString text = block.text();
    QStringRef leftStripped = text.rightRef(text.length() - firstNonSpaceColumn(text));

    if (leftStripped.startsWith(";;;")) {
        return "";
    } else if (leftStripped.startsWith(";;")) {
        // try to align with the next line
        QTextBlock nextBlock = nextNonEmptyBlock(block);
        if (nextBlock.isValid()) {
            return blockIndent(nextBlock);
        }
    }

    TextPosition pos = findBracketBackward('(', TextPosition(block, 0));
    if ( ! pos.isValid()) {
        return QString::null;
    }

    return blockIndent(pos.block) + configuredIndent;
}

};  // namespace Qutepart
