#include <QApplication>

#include "menu_bar.h"


namespace {
QAction* addActionToMenu(QMenu* menu, const QString& text, const QKeySequence& shortcut) {
    QAction* action = menu->addAction(text);
    action->setShortcut(shortcut);
    return action;
}
}

MenuBar::MenuBar(QMainWindow* parent):
    QMenuBar(parent),
    fileMenu(addMenu("File"))
{
    fileOpenAction = addActionToMenu(fileMenu, "Open...", QKeySequence::Open);
    fileSaveAction = addActionToMenu(fileMenu, "Save", QKeySequence::Save);
    fileCloseAction = addActionToMenu(fileMenu, "Close", QKeySequence::Close);

    fileMenu->addSeparator();

    m_fileQuitAction = fileMenu->addAction(
        "Quit",
        QApplication::instance(), SLOT(quit()),
        QKeySequence::Quit);
}

