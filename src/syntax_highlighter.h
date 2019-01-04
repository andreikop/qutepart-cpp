#pragma once

#include <QSyntaxHighlighter>
#include <QTextDocument>

#include "text_block_user_data.h"

class Language;


class SyntaxHighlighter: public QSyntaxHighlighter {
    Q_OBJECT

public:
    SyntaxHighlighter(QTextDocument *parent, Language* language);

protected:
    void  highlightBlock(const QString &text) override;
    Language* language;
};
