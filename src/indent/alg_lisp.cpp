#include "text_block_utils.h"

#include "alg_lisp.h"

namespace Qutepart {

const QString& IndentAlgLisp::triggerCharacters() const {
    static QString ch(';');
    return ch;
}

QString IndentAlgLisp::computeSmartIndent(QTextBlock block, QChar typedKey) const {
    /* special rules: ;;; -> indent 0
                      ;;  -> align with next line, if possible
                      ;   -> usually on the same line as code -> ignore
     */
    QStringRef leftStripped = block.text().rightRef(firstNonSpaceColumn(block.text()));

    if (leftStripped.startsWith(";;;")) {
        return "";
    } else if (leftStripped.startsWith(";;")) {
        // try to align with the next line
        QTextBlock nextBlock = nextNonEmptyBlock(block);
        if (nextBlock.isValid()) {
            return blockIndent(nextBlock);
        }
    }

#if 0
    TextPosition pos = findBracketBackward(block, 0, '(');
    if ( ! pos.isValid()) {
        return "";
    }

    return increseIndent(blockIndent(pos.block));
#endif
    return "";
}

};  // namespace Qutepart
