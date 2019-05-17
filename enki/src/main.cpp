#include <QApplication>
#include <QFile>
#include <QTextCodec>
#include <QDebug>


#include "qutepart.h"


int main(int argc, char** argv) {
    QApplication app(argc, argv);
    Qutepart::Qutepart qutepart;

    QFont font = qutepart.font();
    font.setPointSize(12);
    font.setFamily("Monospace");
    qutepart.setFont(font);

    qutepart.resize(800, 600);

    if (argc > 1) {
        QString filePath = argv[1];
        QFile file(filePath);
        if (file.exists()) {
            qutepart.initHighlighter(filePath);

            file.open(QIODevice::ReadOnly);
            QByteArray data = file.readAll();
            QString text = QTextCodec::codecForUtfText(data, QTextCodec::codecForName("UTF-8"))->toUnicode(data);
            qutepart.setPlainText(text);
        } else {
            qWarning() << "File does not exist" << filePath;
            return -1;
        }
    }


    qutepart.show();

    return app.exec();
}
