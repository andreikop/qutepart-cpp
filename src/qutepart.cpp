#include "qutepart.h"


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

void Qutepart::initHighlighter(const QString& path) {
    Language* language = loadLanguage(path);
    if (language != nullptr) {
        if (highlighter != nullptr) {
            delete highlighter;
        }

        highlighter = new SyntaxHighlighter(this->document(), language);
    }
}
