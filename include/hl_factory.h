#pragma once

#include <QSyntaxHighlighter>


namespace Qutepart {

/*
 * Choose and load a highlighter.
 *
 * Set as much parameters at posiible to detect language correctly
 *
 * See QSyntaxHighlighter(QTextDocument*) documentation.
 */
QSyntaxHighlighter* makeHighlighter(
    QObject* parent,
    const QString& mimeType = QString::null,
    const QString& languageName = QString::null,
    const QString& sourceFilePath = QString::null,
    const QString& firstLine = QString::null);


/*
 * Choose and load a highlighter.
 *
 * Set as much parameters at posiible to detect language correctly
 * The highlighter will be installed automatically for QTextDocument
 *
 * See QSyntaxHighlighter(QTextDocument*) documentation.
 */
QSyntaxHighlighter* makeHighlighter(
    QTextDocument* parent,
    const QString& mimeType = QString::null,
    const QString& languageName = QString::null,
    const QString& sourceFilePath = QString::null,
    const QString& firstLine = QString::null);

};
