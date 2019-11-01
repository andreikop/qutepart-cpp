#pragma once

#include <QString>
#include <QTextBlock>

namespace Qutepart {

int firstNonSpaceColumn(const QString& line);
QString lineIndent(const QString& line);
QString blockIndent(QTextBlock block);
QString prevBlockIndent(QTextBlock block);
QTextBlock prevNonEmptyBlock(QTextBlock block);
QString prevNonEmptyBlockIndent(const QTextBlock& block);
QString textBeforeCursor(QTextCursor cursor);

}
