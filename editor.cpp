#include <stdio.h>

#include <QApplication>
#include <QXmlStreamReader>
#include <QFile>
#include <QDebug>
#include <QDir>

#include "qutepart.h"


int runEditor(int argc, char** argv) {
    QApplication app(argc, argv);
    Qutepart::Qutepart qutepart;

    QString filePath;
    if (argc > 1) {
        filePath = argv[1];
    }

    qutepart.initHighlighter(filePath);
    QFont font = qutepart.font();
    font.setPointSize(18);
    qutepart.setFont(font);

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
