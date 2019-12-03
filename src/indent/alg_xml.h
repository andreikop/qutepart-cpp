#pragma once

#include "indenter.h"


namespace Qutepart {

class IndentAlgXml: public IndentAlgImpl {
public:
    const QString& triggerCharacters() const override;
    QString computeSmartIndent(QTextBlock block, const QString& configuredIndent, QChar typedKey=QChar::Null) const override;

private:
    QString processChar(const QString& lineText, const QString& prevLineText, QChar ch, const QString& configuredIndent) const;
};

};  // namespace Qutepart
