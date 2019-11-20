#include <stdio.h>

#include <QApplication>
#include <QXmlStreamReader>
#include <QFile>
#include <QDebug>
#include <QDir>
#include <QString>
#include <QByteArray>
#include <QTextCodec>

#include "qutepart.h"


int runEditor(int argc, char** argv) {
    Q_INIT_RESOURCE(qutepart_syntax_files);
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
            Qutepart::LangInfo langInfo = Qutepart::chooseLanguage(
                QString::null, QString::null, filePath);
            if (langInfo.isValid()) {
                qutepart.setHighlighter(langInfo.id);
            }

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


#if 0
void parseAllFiles() {
    QDir syntaxFiles("../syntax");
    foreach (QString filePath, syntaxFiles.entryList(QDir::Files)) {
        Language* lang = loadLanguage(syntaxFiles.absoluteFilePath(filePath));

        if (lang != nullptr) {
            delete lang;
        }
    }
}


void showSyntax(const QString& fileName) {
    Language* language = loadLanguage("../syntax/" + fileName);
    if (language == nullptr) {
        return;
    }

    QTextStream out(stdout);
    language->printDescription(out);
    delete language;
}
#endif

int main(int argc, char** argv) {
    QString lang = "zsh";
    if (argc > 1) {
        lang = argv[1];
    }
    return runEditor(argc, argv);
    // parseAllFiles();
    // showSyntax(lang + ".xml");
}
