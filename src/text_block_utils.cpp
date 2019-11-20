#include "text_block_utils.h"

namespace Qutepart {

int firstNonSpaceColumn(const QString& line) {
    for(int i = 0; i < line.size(); i++) {
        if ( ! line[i].isSpace()) {
            return i;
        }
    }
    return line.size();
}

QString lineIndent(const QString& line) {
    return line.left(firstNonSpaceColumn(line));
}

QString blockIndent(QTextBlock block) {
    return lineIndent(block.text());
}

void setBlockIndent(QTextCursor* cursor, const QString& indent) {
    int currentIndentLength = blockIndent(cursor->block()).size();
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

QTextBlock prevNonEmptyBlock(QTextBlock block) {
    if ( ! block.isValid()) {
        return QTextBlock();
    }

    block = block.previous();
    while (block.isValid() && block.text().isEmpty()) {
        block = block.previous();
    }

    return block;
}

QString prevNonEmptyBlockIndent(const QTextBlock& block) {
    return blockIndent(prevNonEmptyBlock(block));
}

QTextBlock nextNonEmptyBlock(QTextBlock block) {
    if ( ! block.isValid()) {
        return block;
    }

    block = block.next();

    while (block.isValid() && block.text().trimmed().isEmpty()) {
        block = block.next();
    }

    return block;
}

QString textBeforeCursor(QTextCursor cursor) {
    QString blockText = cursor.block().text();
    return blockText.left(cursor.positionInBlock());
}


void setPositionInBlock(QTextCursor* cursor, int positionInBlock, QTextCursor::MoveMode anchor) {
    return cursor->setPosition(cursor->block().position() + positionInBlock, anchor);
}

}  // namespace
