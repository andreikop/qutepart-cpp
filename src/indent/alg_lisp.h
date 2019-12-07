#pragma once

#include "indenter.h"


namespace Qutepart {

class IndentAlgLisp: public IndentAlgImpl {
public:
    const QString& triggerCharacters() const override;
    QString computeSmartIndent(QTextBlock block, const QString& configuredIndent) const override;
};

};  // namespace Qutepart
