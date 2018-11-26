#include "qutepart.h"


Qutepart::Qutepart(QWidget *parent):
    QPlainTextEdit(parent),
    highlighter(new SyntaxHighlighter(this->document()))
{};

Qutepart::Qutepart(const QString &text, QWidget *parent):
    QPlainTextEdit(text, parent),
    highlighter(new SyntaxHighlighter(this->document()))
{}

Qutepart::~Qutepart() {
    delete highlighter;
}
