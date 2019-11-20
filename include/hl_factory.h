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
    const QString& languageId);


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
    const QString& id);

};
