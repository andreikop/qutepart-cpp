#include <stdio.h>

#include <QApplication>
#include <QXmlStreamReader>
#include <QFile>
#include <QDebug>

#include "qutepart.h"
#include "loader.h"



int runEditor(int argc, char** argv) {
    QApplication app(argc, argv);
    Qutepart qutepart;
    qutepart.show();

    qWarning() << "exec";
    return app.exec();
}

int main(int /*argc*/, char** /*argv*/) {
    // runEditor(argc, argv);

    QFile syntaxFile("../syntax/rtf.xml");
    if (! syntaxFile.open(QIODevice::ReadOnly | QIODevice::Text) ) {
        qCritical() << "Failed to open syntax file";
        return -1;
    }

    QXmlStreamReader xmlReader(&syntaxFile);

    Language* language = loadLanguage(xmlReader);
    if (language == nullptr) {
        qCritical() << "Failed to parse XML file. " << xmlReader.errorString();
    }

    QTextStream out(stdout);
    language->printDescription(out);
}
