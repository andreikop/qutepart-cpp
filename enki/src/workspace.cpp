#include <QFile>
#include <QTextCodec>
#include <QFileInfo>
#include <QStatusBar>
#include <QDebug>

#include "workspace.h"

Workspace::Workspace(MainWindow& mainWindow):
    m_mainWindow(mainWindow)
{
    connect(this, &Workspace::ioError, mainWindow.statusBar(), &QStatusBar::showMessage);
}

void Workspace::openFile(const QString& filePath, int /*line*/) {
    QFileInfo fileInfo(filePath);

    if ( ! fileInfo.exists()) {
        emit ioError(QString("Failed to open file '%0': File does not exist").arg(filePath));
        qDebug() << filePath;
        return;
    }

    QString canonicalPath = fileInfo.canonicalFilePath();

    QString text = readFile(canonicalPath);
    if (text.isNull()) { // failed to read, error emitted
        return;
    }

    Qutepart::Qutepart* qutepart = new Qutepart::Qutepart(text, &m_mainWindow);
    qutepart->initHighlighter(canonicalPath);

    m_files[canonicalPath] = qutepart;

    m_mainWindow.setCentralWidget(qutepart);
}

QString Workspace::readFile(const QString& filePath) {
    QFile file(filePath);

    bool ok = file.open(QIODevice::ReadOnly);
    if ( ! ok) {
        emit ioError(QString("Failed to open file %0: %1").arg(filePath, file.errorString()));
        return QString::null;
    }

    QByteArray data = file.readAll();
    if (data.isEmpty() && ( ! file.errorString().isEmpty())) {
        emit ioError(QString("Failed to read file %0: %1").arg(filePath, file.errorString()));
        return QString::null;
    }

    return QTextCodec::codecForUtfText(data, QTextCodec::codecForName("UTF-8"))->toUnicode(data);
}
