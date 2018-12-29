#pragma once

#include <QTextCharFormat>
#include <QSharedPointer>


typedef QSharedPointer<QTextCharFormat> FormatPtr;


class Style {
public:
    Style();
    Style(const QString& defStyleName, FormatPtr format);

    /* Called by some clients.
       If the style knows attribute it can better detect textType
     */
    void updateTextType(const QString& attribute);
private:
    FormatPtr format;
    char textType;

    QString defStyleName;
};

Style makeStyle(
        const QString& defStyleName,
        const QString& color,
        const QString& /*selColor*/,
        const QStringList& flags,
        QString& error);
