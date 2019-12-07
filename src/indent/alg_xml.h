#pragma once

#include "indenter.h"


namespace Qutepart {

class IndentAlgXml: public IndentAlgImpl {
public:
    const QString& triggerCharacters() const override;
    QString autoFormatLine(QTextBlock block, const QString& configuredIndent) const override;
    QString computeSmartIndent(QTextBlock block, const QString& configuredIndent) const override;

private:
    QString indentForLine(const QString& lineText, const QString& prevLineText, const QString& configuredIndent) const;
};

};  // namespace Qutepart
