#include <stdio.h>

#include <QApplication>
#include <QXmlStreamReader>
#include <QFile>
#include <QDebug>
#include <QDir>

#include "qutepart.h"
#include "loader.h"


int runEditor(int argc, char** argv) {
    QApplication app(argc, argv);
    Qutepart qutepart;

    QString filePath;
    if (argc > 1) {
        filePath = argv[1];
    }

    qutepart.initHighlighter(filePath);

    qutepart.show();

    qWarning() << "exec";
    return app.exec();
}


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

int main(int argc, char** argv) {
    QString lang = "zsh";
    if (argc > 1) {
        lang = argv[1];
    }
    return runEditor(argc, argv);
    // parseAllFiles();
    // showSyntax(lang + ".xml");
}
