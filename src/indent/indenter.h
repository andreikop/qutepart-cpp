#pragma once

#include <QString>
#include <QTextBlock>
#include <QKeyEvent>

namespace Qutepart {

class IndentAlg {
public:
    virtual const QString& triggerCharacters() const;
    virtual QString computeSmartIndent(QTextBlock block, QChar typedKey) const = 0;
};

class Indenter {
public:
    Indenter();
    virtual ~Indenter();

    QString text() const;

    bool shouldAutoIndentOnEvent(QKeyEvent* event) const;
    bool shouldUnindentWithBackspace(const QTextCursor& cursor) const;
#if 0
    void autoIndentBlock(QTextBlock block, QChar typedKey) const;
#endif
    QString indentForBlock(QTextBlock block, QChar typedKey) const;

    void onShortcutUnindentWithBackspace(QTextCursor& cursor) const;
private:
    IndentAlg* alg;

};
};
