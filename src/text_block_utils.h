#pragma once

#include <QString>
#include <QTextBlock>

namespace Qutepart {

struct TextPosition {
public:
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

QString lineIndent(const QString& line);
QString blockIndent(QTextBlock block);
void setBlockIndent(QTextCursor* cursor, const QString& indent);
QString prevBlockIndent(QTextBlock block);

QTextBlock prevNonEmptyBlock(QTextBlock block);
QString prevNonEmptyBlockIndent(const QTextBlock& block);

QTextBlock nextNonEmptyBlock(QTextBlock block);

QString textBeforeCursor(QTextCursor cursor);

void setPositionInBlock(
    QTextCursor* cursor,
    int positionInBlock,
    QTextCursor::MoveMode anchor=QTextCursor::MoveAnchor);
}
