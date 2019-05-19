#pragma once

#include <QObject>
#include <QString>

#include "qutepart.h"

#include "main_window.h"

/* Workspace is responsible for opening and switching files
 */

struct Editor {
    Editor(const QString& filePath, const QString& text, QMainWindow* parent);

    QString filePath;
    Qutepart::Qutepart qutepart;
};


class Workspace: public QObject {
    Q_OBJECT
public:
    Workspace(MainWindow& mainWindow);
    virtual ~Workspace();

    void openFile(const QString& filePath, int line=-1);

private:
    QList<Editor*> m_files;
    QMainWindow& m_mainWindow;

    QString readFile(const QString& filePath);
    void showError(const QString& header, const QString& text);

private slots:
    void onFileOpen();
    void onFileSave();
    void onFileClose();
};
