#pragma once

#include <QPlainTextEdit>
#include <QSharedPointer>
#include <QSyntaxHighlighter>


namespace Qutepart {

class Qutepart: public QPlainTextEdit {
public:
    Qutepart(QWidget *parent = nullptr);
    Qutepart(const QString &text, QWidget *parent = nullptr);

    void initHighlighter(const QString& filePath);
private:
    QSharedPointer<QSyntaxHighlighter> highlighter;
};

};
