#include <QApplication>
#include <QFile>
#include <QDebug>


#include "qutepart.h"

#include "main_window.h"
#include "workspace.h"


int main(int argc, char** argv) {
    QApplication app(argc, argv);

    MainWindow mainWindow;
    Workspace workspace(mainWindow);

    if (argc > 1) {
        QString filePath = argv[1];
        if (QFile(filePath).exists()) {
            workspace.openFile(filePath);
        } else {
            qWarning() << "File does not exist" << filePath;
            return -1;
        }
    }

    mainWindow.show();

    return app.exec();
}
