#pragma once

#include <QString>
#include <QTextBlock>
#include <QKeyEvent>

namespace Qutepart {

class IndentAlg {
public:
    virtual const QString& triggerCharacters() const = 0;
    virtual QString computeSmartIndent(QTextBlock block, QChar typedKey) const = 0;
};

class Indenter {
public:
    Indenter();
    virtual ~Indenter();

    bool shouldAutoIndentOnEvent(QKeyEvent* event) const;
#if 0
    void autoIndentBlock(QTextBlock block, QChar typedKey) const;
#endif
    QString indentForBlock(QTextBlock block, QChar typedKey) const;

private:
    IndentAlg* alg;

};
};
