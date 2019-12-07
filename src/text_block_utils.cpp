#include <utility>
#include <map>

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
{}

QChar BackwardCharIterator::step() {
    if ( ! atEnd()) {
        movePositionBack();
        QChar retVal = position_.block.text()[position_.column];
        return retVal;
    } else {
        return QChar::Null;
    }
}

TextPosition BackwardCharIterator::currentPosition() const {
    return position_;
}

bool BackwardCharIterator::atEnd() const {
    return position_.column == 0 &&
           ( ! position_.block.isValid());
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

TextPosition findAnyOpeningBracketBackward(const TextPosition& pos) {
    std::map<std::pair<QChar, QChar>, int> depth;

    depth[std::make_pair('(', ')')] = 1;
    depth[std::make_pair('[', ']')] = 1;
    depth[std::make_pair('{', '}')] = 1;

    BackwardCharIterator it(pos);

    while ( ! it.atEnd()) {
        QChar ch = it.step();
        // if self._qpart.isCode(foundBlock.blockNumber(), foundColumn):

        for (auto mapIt = depth.begin(); mapIt != depth.end(); ++mapIt) {
            QChar opening = mapIt->first.first;
            QChar closing = mapIt->first.second;

            if (ch == opening) {
                mapIt->second--;
                if (mapIt->second == 0) {
                    return it.currentPosition();
                }
            } else if (ch == closing) {
                mapIt->second++;
            }
        }
    }

    return TextPosition();
}

}  // namespace Qutepart
