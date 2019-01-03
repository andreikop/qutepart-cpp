#pragma once

#include <QSyntaxHighlighter>
#include <QTextDocument>

#include "text_block_user_data.h"


class SyntaxHighlighter: public QSyntaxHighlighter {
    Q_OBJECT

public:
    SyntaxHighlighter(QTextDocument *parent = 0);

protected:
    void  highlightBlock(const QString &text) override;
};
