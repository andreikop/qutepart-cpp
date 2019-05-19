#pragma once

#include <QMainWindow>
#include <QMenuBar>


class MenuBar: public QMenuBar {
public:
    MenuBar(QMainWindow* parent);

    QMenu* fileMenu;

    QAction* fileOpenAction;
    QAction* fileSaveAction;
    QAction* fileCloseAction;
private:

    QAction* m_fileQuitAction;
};
