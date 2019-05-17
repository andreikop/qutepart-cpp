#include <QFile>
#include <QTextCodec>
#include <QDebug>

#include "workspace.h"

Workspace::Workspace(MainWindow& mainWindow):
    m_mainWindow(mainWindow)
{}

void Workspace::openFile(const QString& filePath, int /*line*/) {
    QFile file(filePath);
    if (file.exists()) {
        Qutepart::Qutepart* qutepart = new Qutepart::Qutepart(&m_mainWindow);
        qutepart->initHighlighter(filePath);

        file.open(QIODevice::ReadOnly);
        QByteArray data = file.readAll();
        QString text = QTextCodec::codecForUtfText(data, QTextCodec::codecForName("UTF-8"))->toUnicode(data);
        qutepart->setPlainText(text);

        m_files[filePath] = qutepart;

        m_mainWindow.setCentralWidget(qutepart);
    } else {
        qWarning() << "File does not exist" << filePath;
        return;
    }
}
