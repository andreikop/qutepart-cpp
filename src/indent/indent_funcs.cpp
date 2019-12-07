#include "text_block_utils.h"

#include "indent_funcs.h"

namespace Qutepart {


QString lineIndent(const QString& line) {
    return line.left(firstNonSpaceColumn(line));
}

QString blockIndent(QTextBlock block) {
    return lineIndent(block.text());
}

void setBlockIndent(QTextCursor* cursor, const QString& indent) {
    int currentIndentLength = blockIndent(cursor->block()).size();
    setPositionInBlock(cursor, 0, QTextCursor::MoveAnchor);
    setPositionInBlock(cursor, currentIndentLength, QTextCursor::KeepAnchor);
    cursor->insertText(indent);
}

QString prevBlockIndent(QTextBlock block) {
    QTextBlock prevBlock = block.previous();

    if ( ! block.isValid()) {
            return QString::null;
    }

    return blockIndent(prevBlock);
}

QString increaseIndent(const QString& line, const QString& indent) {
    return indent + line;
}

QString decreaseIndent(const QString& line, const QString& indent) {
    if (line.startsWith(indent)) {
        return line.mid(indent.length());
    } else {  // oops, strange indentation, just return previous indent
        return line;
    }
}

/* Make indent text with specified with.
 * Contains width count of spaces, or tabs and spaces
 */
QString makeIndentFromWidth(int width, bool confWidth, bool confUseTabs) {
    if (confUseTabs) {
        int tabCount = width / confWidth;
        int spaceCount = width % confWidth;
        return QString().fill('\t', tabCount) + QString().fill(' ', spaceCount);
    } else {
        return QString().fill(' ', width);
    }
}

QString makeIndentAsColumn(
        QTextBlock block, int column,
        int confIndentWidth, bool confUseTabs,
        int offset) {
    QString blockText = block.text();
    QString textBeforeColumn = blockText.left(column);
    int tabCount = textBeforeColumn.count('\t');

    int visibleColumn = column + (tabCount * (confIndentWidth - 1));
    return makeIndentFromWidth(visibleColumn + offset, confIndentWidth, confUseTabs);
}


QString prevNonEmptyBlockIndent(const QTextBlock& block) {
    return blockIndent(prevNonEmptyBlock(block));
}

};  // namespace Qutepart
