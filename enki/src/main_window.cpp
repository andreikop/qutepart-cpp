#include <QFont>

#include "main_window.h"

MainWindow::MainWindow() {
    setMenuBar(new MenuBar(this));
    resize(800, 600);

    QFont font = this->font();
    font.setPointSize(12);
    font.setFamily("Monospace");
    setFont(font);

}

MenuBar* MainWindow::menuBar() {
    return dynamic_cast<MenuBar*>(QMainWindow::menuBar());
}
