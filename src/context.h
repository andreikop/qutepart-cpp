#pragma once

#include <QTextStream>
#include <QSharedPointer>


class Context;
typedef QSharedPointer<Context> ContextPtr;
class ContextSwitcher;

class AbstractRule;
typedef QSharedPointer<AbstractRule> RulePtr;


class ContextSwitcher {
public:
    ContextSwitcher();
    ContextSwitcher(int popsCount, ContextPtr contextToSwitch, const QString& contextOperation);

    QString toString() const;
    bool isNull() const;

protected:
    int popsCount;
    ContextPtr contextToSwitch;
    QString contextOperation;

};


class Context {
public:
    Context(const QString& name,
            const QString& attribute,
            const ContextSwitcher& lineEndContext,
            const ContextSwitcher& lineBeginContext,
            const ContextSwitcher& fallthroughContext,
            bool dynamic,
            const QList<RulePtr>& rules);

    void printDescription(QTextStream& out);

protected:
    QString name;
    QString attribute;
    ContextSwitcher lineEndContext;
    ContextSwitcher lineBeginContext;
    ContextSwitcher fallthroughContext;
    bool dynamic;
    QList<RulePtr> rules;
};
