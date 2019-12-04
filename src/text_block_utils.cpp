#include <QDebug>

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

int lastNonSpaceColumn(const QString& line) {
    for(int i = line.size() - 1; i >= 0; i--) {
        if ( ! line[i].isSpace()) {
            return i;
        }
    }
    return 0;
}

QString stripLeftWhitespace(const QString& line) {
    return line.mid(firstNonSpaceColumn(line));
}

QString stripRightWhitespace(const QString& line) {
    return line.left(lastNonSpaceColumn(line));
}

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

BackwardCharIterator::BackwardCharIterator(const TextPosition& position):
        position_(position)
{
    movePositionBack();
}

QChar BackwardCharIterator::step() {
    if ( ! atEnd()) {
        QChar retVal = position_.block.text()[position_.column];
        movePositionBack();
        return retVal;
    } else {
        return QChar::Null;
    }
}

TextPosition BackwardCharIterator::currentPosition() const {
    return position_;
}

bool BackwardCharIterator::atEnd() const {
    return ! position_.isValid();
}

void BackwardCharIterator::movePositionBack() {
    while (1) {
        if (position_.column > 0) {
            position_.column--;
            break;
        } else {
            position_.block = position_.block.previous();
            if ( ! position_.block.isValid()) {
                break;
            }

            position_.column = position_.block.length();
            /* move block backward, but the block might be empty
               Go to next while loop iteration and move back
               more blocks if necessary
             */
        }
    }
}

TextPosition findBracketBackward(QChar bracket, const TextPosition& position) {
    QChar opening = QChar::Null;
    QChar closing = QChar::Null;

    if (bracket == '(' || bracket == ')') {
        opening = '(';
        closing = ')';
    } else if (bracket == '[' || bracket == ']') {
        opening = '[';
        closing = ']';
    } else if (bracket == '{' || bracket == '}') {
        opening = '{';
        closing = '}';
    } else {
        qDebug() << "Invalid bracket" << bracket;
        return TextPosition();
    }

    int depth = 1;

    BackwardCharIterator it(position);
    while ( ! it.atEnd()) {
        QChar ch = it.step();
        // TODO if not self._qpart.isComment(foundBlock.blockNumber(), foundColumn):
        if (ch == opening) {
            depth--;
        }
        else if (ch == closing) {
            depth++;
        }

        if (depth == 0) {
            return it.currentPosition();
        }
    }

    return TextPosition();
}

}  // namespace Qutepart
