#pragma once

#include <QString>
#include <QTextBlock>

namespace Qutepart {

int firstNonSpaceColumn(const QString& line);

QString lineIndent(const QString& line);
QString blockIndent(QTextBlock block);
void setBlockIndent(QTextCursor* cursor, const QString& indent);
QString prevBlockIndent(QTextBlock block);

QTextBlock prevNonEmptyBlock(QTextBlock block);
QString prevNonEmptyBlockIndent(const QTextBlock& block);
QString textBeforeCursor(QTextCursor cursor);

void setPositionInBlock(
    QTextCursor* cursor,
    int positionInBlock,
    QTextCursor::MoveMode anchor=QTextCursor::MoveAnchor);
}
