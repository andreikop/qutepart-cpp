#include "text_to_match.h"


TextToMatch::TextToMatch(const QString& text, const QString& deliminatorSet):
    currentColumnIndex(0),
    wholeLineText(text),
    text(wholeLineText.leftRef(wholeLineText.length())),
    textLength(text.length()),
    firstNonSpace(true), // copy-paste from Py code
    isWordStart(true), // copy-paste from Py code
    deliminatorSet(deliminatorSet)
{
    findWord();
}

void TextToMatch::findWord() {
    int wordEndIndex = 0;
    for(; wordEndIndex < text.length(); wordEndIndex++) {
        if (deliminatorSet.contains(text.at(wordEndIndex))) {
            break;
        }
    }
    if (wordEndIndex != 0) {
        word = text.left(wordEndIndex);
    }
}

void TextToMatch::shiftWord(int count) {
    if(word.length() > count) {
        word = word.right(word.length() - count);
    } else if (word.length() == count) {
        word = QStringRef();
    } else {
        findWord(); // TODO do upon request?
    }
}

void TextToMatch::shiftOnce() {
    QChar prevChar = text.at(0);
    firstNonSpace = firstNonSpace && prevChar.isSpace();
    isWordStart = prevChar.isSpace() || deliminatorSet.contains(prevChar);

    currentColumnIndex++;
    text = text.right(text.length() - 1);
    textLength--;

    shiftWord(1);
}

void TextToMatch::shift(int count) {
    for(int i = 0; i < count; i++) {
        QChar prevChar = text.at(i);
        firstNonSpace = firstNonSpace && prevChar.isSpace();
        isWordStart = prevChar.isSpace() || deliminatorSet.contains(prevChar);
    }

    currentColumnIndex += count;
    text = text.right(text.length() - count);
    textLength -= count;
    shiftWord(count);
}

bool TextToMatch::isEmpty() const {
    return text.isEmpty();
}
