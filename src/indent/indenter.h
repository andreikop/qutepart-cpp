#pragma once

#include <QObject>
#include <QString>
#include <QTextBlock>
#include <QKeyEvent>

namespace Qutepart {

class IndentAlg {
public:
    virtual const QString& triggerCharacters() const;
    virtual QString computeSmartIndent(QTextBlock block, QChar typedKey=QChar::Null) const = 0;
};


class Indenter: public QObject {
public:
    Indenter();
    virtual ~Indenter();

    QString text() const;

    int width() const;
    void setWidth(int);

    bool useTabs() const;
    void setUseTabs(bool);

    bool shouldAutoIndentOnEvent(QKeyEvent* event) const;
    bool shouldUnindentWithBackspace(const QTextCursor& cursor) const;
#if 0
    void autoIndentBlock(QTextBlock block, QChar typedKey) const;
#endif
    QString indentForBlock(QTextBlock block, QChar typedKey) const;

public slots:
    void onShortcutIndent(QTextCursor cursor) const;
    void onShortcutUnindentWithBackspace(QTextCursor& cursor) const;

private:
    IndentAlg* alg;
    bool useTabs_;
    int width_;
};
};
