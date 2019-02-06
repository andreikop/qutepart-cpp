#pragma once

#include <memory>

#include <QSyntaxHighlighter>
#include <QTextDocument>

#include "text_block_user_data.h"
#include "language.h"

namespace Qutepart {


class SyntaxHighlighter: public QSyntaxHighlighter {
    Q_OBJECT

public:
    SyntaxHighlighter(QTextDocument *parent, std::unique_ptr<Language>& language);

protected:
    void  highlightBlock(const QString &text) override;
    std::unique_ptr<Language> language;
};

}
