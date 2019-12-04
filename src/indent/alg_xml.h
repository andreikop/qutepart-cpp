#pragma once

#include "indenter.h"


namespace Qutepart {

class IndentAlgXml: public IndentAlgImpl {
public:
    const QString& triggerCharacters() const override;
    QString computeIndentedLine(QTextBlock block, const QString& configuredIndent, QChar typedKey=QChar::Null) const override;

private:
    QString indentForLine(const QString& lineText, const QString& prevLineText, const QString& configuredIndent) const;
};

};  // namespace Qutepart
