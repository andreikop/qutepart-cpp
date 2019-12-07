#pragma once

#include <QString>
#include <QTextBlock>

#include "text_block_utils.h"

#include "indenter.h"


namespace Qutepart {

class IndentAlgPython: public IndentAlgImpl {
public:
    QString computeSmartIndent(QTextBlock block, const QString& configuredIndent) const override;

private:
    QString computeSmartIndent(
        const TextPosition& pos,
        const QString& configuredIndent) const;
};

};  // namespace Qutepart
