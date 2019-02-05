#include "qutepart.h"
#include "hl/language_db.h"
#include "hl/loader.h"
#include "hl/syntax_highlighter.h"


namespace Qutepart {

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

void Qutepart::initHighlighter(const QString& filePath) {
    const QString& xmlFileName = chooseLanguage(QString::null, QString::null, filePath, QString::null);

    if (xmlFileName.isNull()) {
        return;
    }

    Language* language = loadLanguage(xmlFileName);
    if (language != nullptr) {
        if (highlighter != nullptr) {
            delete highlighter;
        }

        highlighter = new SyntaxHighlighter(this->document(), language);
    }
}

}
