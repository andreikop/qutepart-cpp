#pragma once

#include <QTextStream>
#include <QSharedPointer>
#include <QHash>

#include "style.h"
#include "context_stack.h"
#include "context_switcher.h"


class Context;
typedef QSharedPointer<Context> ContextPtr;

class AbstractRule;
typedef QSharedPointer<AbstractRule> RulePtr;


class Context {
public:
    Context(const QString& name,
            const QString& attribute,
            const ContextSwitcher& lineEndContext,
            const ContextSwitcher& lineBeginContext,
            const ContextSwitcher& fallthroughContext,
            bool dynamic,
            const QList<RulePtr>& rules);

    void printDescription(QTextStream& out) const;

    QString name() const;

    void resolveContextReferences(const QHash<QString, ContextPtr>& contexts, QString& error);
    void setKeywordParams(const QHash<QString, QStringList>& lists,
                          const QString& deliminators,
                          bool caseSensitive,
                          QString& error);
    void setStyles(const QHash<QString, Style>& styles, QString& error);

    bool dynamic() const {return _dynamic;};

protected:
    QString _name;
    QString attribute;
    ContextSwitcher lineEndContext;
    ContextSwitcher lineBeginContext;
    ContextSwitcher fallthroughContext;
    bool _dynamic;
    QList<RulePtr> rules;

    Style style;
};
