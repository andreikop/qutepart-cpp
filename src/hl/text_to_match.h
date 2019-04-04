#pragma once


#include <QString>


namespace Qutepart {

/* Peace of text, which shall be matched.
 * Contains pre-calculated and pre-checked data for performance optimization
 */
class TextToMatch {
public:
    TextToMatch(const QString& text, const QStringList& contextData);

    void shiftOnce();
    void shift(int count);

    bool isEmpty() const;

    QString word(const QString& deliminators) const;

    int currentColumnIndex;
    QString wholeLineText;
    QStringRef text;
    int textLength;
    bool firstNonSpace;
    bool isWordStart;
    const QStringList* contextData;
};

};
