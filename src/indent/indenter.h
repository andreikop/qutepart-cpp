#pragma once

#include <memory>

#include <QObject>
#include <QString>
#include <QTextBlock>
#include <QKeyEvent>

#include "qutepart.h"


namespace Qutepart {

class IndentAlgImpl {
public:
    virtual const QString& triggerCharacters() const;
    virtual QString computeSmartIndent(QTextBlock block, QChar typedKey=QChar::Null) const = 0;
};


class Indenter: public QObject {
public:
    Indenter();

    void setAlgorithm(IndentAlg alg);

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
    std::unique_ptr<IndentAlgImpl> alg_;
    bool useTabs_;
    int width_;
};
};
