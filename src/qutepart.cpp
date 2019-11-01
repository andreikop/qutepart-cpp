#include <QAction>
#include <QMetaMethod>
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
    initActions();
    setAttribute(Qt::WA_KeyCompression, false);  // vim can't process compressed keys
};

Qutepart::Qutepart(const QString &text, QWidget *parent):
    QPlainTextEdit(text, parent)
{
    initActions();
    setAttribute(Qt::WA_KeyCompression, false);  // vim can't process compressed keys
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
        // make action shortcuts override keyboard events (non-default Qt behaviour)
        foreach(QAction* action, actions()) {
            QKeySequence seq = action->shortcut();
            if (seq.count() == 1 && seq[0] == (event->key() | int(event->modifiers()))) {
                action->trigger();
                return;
            }
        }

        QPlainTextEdit::keyPressEvent(event);
    }
}

void Qutepart::initActions() {
    connect(
        createAction( "Increase indentation", QKeySequence(Qt::Key_Tab)),
        &QAction::triggered,
        [=]{indenter_.onShortcutIndent(textCursor());}
    );
}

QAction* Qutepart::createAction(
    const QString& text, QKeySequence shortcut,
    const QString& iconFileName) {

    QAction* action = new QAction(text, this);
    // if iconFileName is not None:
    //     action.setIcon(getIcon(iconFileName))

    action->setShortcut(shortcut);
    action->setShortcutContext(Qt::WidgetShortcut);

    addAction(action);
    return action;
}

}
