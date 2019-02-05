#pragma once

#include <QPlainTextEdit>


namespace Qutepart {

class SyntaxHighlighter;

class Qutepart: public QPlainTextEdit {
public:
    Qutepart(QWidget *parent = nullptr);
    Qutepart(const QString &text, QWidget *parent = nullptr);

    virtual ~Qutepart();

    void initHighlighter(const QString& filePath);
private:
    SyntaxHighlighter* highlighter;
};

};
