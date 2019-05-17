#pragma once

#include <QString>

#include "qutepart.h"

#include "main_window.h"

/* Workspace is responsible for opening and switching files
 */

class Workspace {
public:
    Workspace(MainWindow& mainWindow);

    void openFile(const QString& filePath, int line=-1);

private:
    QMap<QString, Qutepart::Qutepart*> m_files;
    QMainWindow& m_mainWindow;
};
