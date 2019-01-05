#pragma once


#include <QString>

/* Peace of text, which shall be matched.
 * Contains pre-calculated and pre-checked data for performance optimization
 */
class TextToMatch {
public:
    TextToMatch(const QString& text, const QString& deliminatorSet);

    void shiftOnce();
    void shift(int count);

    int currentColumnIndex;
    QString wholeLineText;
    QStringRef text;
    int textLength;
    bool firstNonSpace;
    bool isWordStart;
    QStringRef word;

private:
    void shiftWord(int count);
    void findWord(); // find word by scanning `text` char by char
    const QString& deliminatorSet;
};
