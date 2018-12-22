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
    qutepart.show();

    qWarning() << "exec";
    return app.exec();
}

Language* loadLanguage(const QString& filePath) {
    QFile syntaxFile(filePath);
    if (! syntaxFile.open(QIODevice::ReadOnly | QIODevice::Text) ) {
        qCritical() << "Failed to open syntax file " << filePath;
        return nullptr;
    }

    QXmlStreamReader xmlReader(&syntaxFile);

    QString error;
    Language* language = loadLanguage(xmlReader, error);
    if (language == nullptr) {
        qCritical() << "Failed to parse XML file '" << filePath << "': " << error;
        return nullptr;
    }

    return language;
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
    // return runEditor(argc, argv);
    // parseAllFiles();
    showSyntax(lang + ".xml");
}
