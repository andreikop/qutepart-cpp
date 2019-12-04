#include "text_block_utils.h"

#include "alg_scheme.h"

namespace Qutepart {


namespace {
/* Move backward to the start of the word at the end of a string.
 * Return the word
 */
QString lastWord(const QString& text) {
    for(int i = 0; i < text.length(); i++) {
        QChar ch = text[i];
        if (ch.isSpace() ||
            ch == '(' ||
            ch == ')') {
            return text.mid(text.length() - i);
        }
    }

    return text;
}

// Find end of the last expression
TextPosition findExpressionEnd(QTextBlock block) {
    while (block.isValid()) {
        int column = lastNonSpaceColumn(block.text());
        if (column > 0) {
            return TextPosition(block, column);
        }
        block = block.previous();
    }

    return TextPosition();
}

// Find start of not finished expression
TextPosition findExpressionStart(QTextBlock block) {
    TextPosition expEnd = findExpressionEnd(block);

    QString text = expEnd.block.text().left(expEnd.column + 1);
    if (text.endsWith(")")) {
        return findBracketBackward('(', expEnd);
    } else {
        return TextPosition(expEnd.block, text.length() - lastWord(text).length());
    }
}

}; // anonymous namespace


QString IndentAlgScheme::computeSmartIndent(
        QTextBlock block,
        const QString& configuredIndent,
        QChar typedKey) const {
    TextPosition pos = findExpressionStart(block.previous());

    if (! pos.isValid()) {
        return "";
    }

    QString blockText = pos.block.text();
    QString expression = stripRightWhitespace(blockText.mid(pos.column));
    QString beforeExpression = blockText.left(pos.column).trimmed();

    if (beforeExpression.startsWith("(module")) {  // special case
        return "";
    } else if (beforeExpression.endsWith("define")) {  // special case
        return QString().fill(' ', beforeExpression.length() - QString("define").length() + 1);
    } else if (beforeExpression.endsWith("let")) {  // special case
        return QString().fill(' ', beforeExpression.length() - QString("let").length() + 1);
    } else {
        return QString().fill(' ', pos.column);
    }
}

};  // namespace Qutepart
