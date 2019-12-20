#pragma once

#include <QPlainTextEdit>
#include <QList>

#include "text_block_utils.h"

namespace Qutepart {

class BracketHighlighter {
    QList<QTextEdit::ExtraSelection> extraSelections(const TextPosition& pos);

private:
    QList<QTextEdit::ExtraSelection> highlightBracket(QChar bracket, const TextPosition& pos);
    TextPosition cachedBracket_;
    TextPosition cachedMatchingBracket_;
};

};  // namespace Qutepart
