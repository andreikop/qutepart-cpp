#pragma once

#include "indenter.h"


namespace Qutepart {

class IndentAlgLisp: public IndentAlgImpl {
public:
    const QString& triggerCharacters() const override;
    QString computeSmartIndent(QTextBlock block, QChar typedKey=QChar::Null) const override;
};

};  // namespace Qutepart
