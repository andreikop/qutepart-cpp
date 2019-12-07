#pragma once

#include <QString>
#include <QTextBlock>

namespace Qutepart {

struct TextPosition {
public:
    inline TextPosition():
        column(-1)
    {};

    inline TextPosition(QTextBlock block_, int column_):
        block(block_),
        column(column_)
    {};

    inline bool isValid() const {
        return block.isValid();
    }

    QTextBlock block;
    int column;
};

int firstNonSpaceColumn(const QString& line);
int lastNonSpaceColumn(const QString& line);

QString stripLeftWhitespace(const QString& line);
QString stripRightWhitespace(const QString& line);

QTextBlock prevNonEmptyBlock(QTextBlock block);
QTextBlock nextNonEmptyBlock(QTextBlock block);

QString textBeforeCursor(QTextCursor cursor);

void setPositionInBlock(
    QTextCursor* cursor,
    int positionInBlock,
    QTextCursor::MoveMode anchor=QTextCursor::MoveAnchor);

class BackwardCharIterator {
public:
    // create iterator and make first step back
    BackwardCharIterator(const TextPosition& position);

    QChar step();  // return current character and then make step back
    TextPosition currentPosition() const;
    bool atEnd() const;

private:
    TextPosition position_;

    void movePositionBack();
};

/* find bracket backward from position (not including position)
   Return invalid position if not found
   NOTE this function ignores comments
 */
TextPosition findBracketBackward(QChar bracket, const TextPosition& position);

/* Search for opening bracket. Ignores balanced bracket pairs

    (a + (b + c)
    ^
    this bracket will be found

NOTE this methods ignores strings and comments
TODO currently it doesn't
 */
TextPosition findAnyOpeningBracketBackward(const TextPosition& pos);

}  // namespace Qutepart
