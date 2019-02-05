#pragma once


#include <QString>


namespace Qutepart {

/* Peace of text, which shall be matched.
 * Contains pre-calculated and pre-checked data for performance optimization
 */
class TextToMatch {
public:
    TextToMatch(const QString& text, const QString& deliminatorSet, const QStringList& contextData);

    void shiftOnce();
    void shift(int count);

    bool isEmpty() const;

    int currentColumnIndex;
    QString wholeLineText;
    QStringRef text;
    int textLength;
    bool firstNonSpace;
    bool isWordStart;
    QString word;
    const QStringList* contextData;

private:
    void shiftWord(int count);
    void findWord(); // find word by scanning `text` char by char
    const QString& deliminatorSet;
};

};
