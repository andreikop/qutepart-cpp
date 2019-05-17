#include <QFont>

#include "main_window.h"

MainWindow::MainWindow() {
    resize(800, 600);

    QFont font = this->font();
    font.setPointSize(12);
    font.setFamily("Monospace");
    setFont(font);
}
