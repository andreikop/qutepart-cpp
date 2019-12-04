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

    /* Compute line with proper indentation
     * This method has default implementation which calls
     * computeSmartIndent()
     * but can be overriden to modify string
     */
    virtual QString computeIndentedLine(
        QTextBlock block,
        const QString& configuredIndent,
        QChar typedKey=QChar::Null) const;

    /* Compute indent for line.
     * The majority of algorithms should override this method.
     * Default implementation returns empty string
     */
    virtual QString computeSmartIndent(
        QTextBlock block,
        const QString& configuredIndent,
        QChar typedKey=QChar::Null) const;
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
    void indentBlock(QTextBlock block, QChar typedKey) const;

public slots:
    void onShortcutIndent(QTextCursor cursor) const;
    void onShortcutUnindentWithBackspace(QTextCursor& cursor) const;

private:
    std::unique_ptr<IndentAlgImpl> alg_;
    bool useTabs_;
    int width_;
};
};
