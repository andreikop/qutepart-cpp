#pragma once

#include <QSyntaxHighlighter>
#include <QTextDocument>


class SyntaxHighlighter: public QSyntaxHighlighter {
    Q_OBJECT

public:
    SyntaxHighlighter(QTextDocument *parent = 0);

protected:
    void  highlightBlock(const QString &text) override;
};
