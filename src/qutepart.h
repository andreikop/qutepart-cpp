#pragma once

#include <QPlainTextEdit>

#include "syntax_highlighter.h"


class Qutepart: public QPlainTextEdit {
public:
    Qutepart(QWidget *parent = nullptr);
    Qutepart(const QString &text, QWidget *parent = nullptr);

    virtual ~Qutepart();

private:
    SyntaxHighlighter* highlighter;
};
