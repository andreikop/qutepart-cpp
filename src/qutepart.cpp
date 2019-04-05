#include <QDebug>

#include "qutepart.h"
#include "hl_factory.h"

#include "hl/language_db.h"
#include "hl/loader.h"
#include "hl/syntax_highlighter.h"


namespace Qutepart {

Qutepart::Qutepart(QWidget *parent):
    QPlainTextEdit(parent)
{
};

Qutepart::Qutepart(const QString &text, QWidget *parent):
    QPlainTextEdit(text, parent)
{
}

void Qutepart::initHighlighter(const QString& filePath) {
    highlighter = QSharedPointer<QSyntaxHighlighter>(makeHighlighter(document(), QString::null, QString::null, filePath, QString::null));
}

}
