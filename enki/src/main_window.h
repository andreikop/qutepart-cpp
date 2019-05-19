#pragma once

#include <QMainWindow>

#include "menu_bar.h"


class MainWindow: public QMainWindow {
public:
    MainWindow();
    MenuBar* menuBar();
};
