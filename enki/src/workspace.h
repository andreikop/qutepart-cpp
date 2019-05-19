#pragma once

#include <QObject>
#include <QString>

#include "qutepart.h"

#include "main_window.h"

/* Workspace is responsible for opening and switching files
 */

class Workspace: public QObject {
    Q_OBJECT
public:
    Workspace(MainWindow& mainWindow);

    void openFile(const QString& filePath, int line=-1);

private:
    QMap<QString, Qutepart::Qutepart*> m_files;
    QMainWindow& m_mainWindow;

    QString readFile(const QString& filePath);
    void showError(const QString& header, const QString& text);
};
