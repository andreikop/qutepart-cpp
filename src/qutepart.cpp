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
    highlighter_ = QSharedPointer<QSyntaxHighlighter>(makeHighlighter(document(), QString::null, QString::null, filePath, QString::null));
}

void Qutepart::keyPressEvent(QKeyEvent *event) {
    QTextCursor cursor = textCursor();
    if (event->key() == Qt::Key_Backspace &&
        indenter_.shouldUnindentWithBackspace(cursor)) {
        indenter_.onShortcutUnindentWithBackspace(cursor);
    } else if (event->matches(QKeySequence::InsertParagraphSeparator)) {
        QPlainTextEdit::keyPressEvent(event);
        QString indent = indenter_.indentForBlock(cursor.block(), event->text()[0]);
        if ( ! indent.isNull()) {
            cursor.insertText(indent);
        }
    } else if (cursor.atEnd() && indenter_.shouldAutoIndentOnEvent(event)) {
        QPlainTextEdit::keyPressEvent(event);
        QString indent = indenter_.indentForBlock(cursor.block(), event->text()[0]);
        if ( ! indent.isNull()) {
            cursor.insertText(indent);
        }
    } else {
        QPlainTextEdit::keyPressEvent(event);
    }
}

}
