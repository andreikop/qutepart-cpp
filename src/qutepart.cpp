#include "qutepart.h"
#include "language_db.h"


Qutepart::Qutepart(QWidget *parent):
    QPlainTextEdit(parent),
    highlighter(nullptr)
{
};

Qutepart::Qutepart(const QString &text, QWidget *parent):
    QPlainTextEdit(text, parent),
    highlighter(nullptr)
{
}

Qutepart::~Qutepart() {
    if (highlighter != nullptr) {
        delete highlighter;
    }
}

// FIXME replace this func with better loader
Language* loadLanguage(const QString& filePath);

void Qutepart::initHighlighter(const QString& filePath) {
    const QString& xmlFileName = chooseLanguage(QString::null, QString::null, filePath, QString::null);

    if (xmlFileName.isNull()) {
        return;
    }

    QString xmlFilePath = ":/qutepart/syntax/" + xmlFileName;
    Language* language = loadLanguage(xmlFilePath);
    if (language != nullptr) {
        if (highlighter != nullptr) {
            delete highlighter;
        }

        highlighter = new SyntaxHighlighter(this->document(), language);
    }
}
