#pragma once

#include <QTextStream>
#include <QSharedPointer>
#include <QHash>


class Context;
typedef QSharedPointer<Context> ContextPtr;
class ContextSwitcher;

class AbstractRule;
typedef QSharedPointer<AbstractRule> RulePtr;


class ContextSwitcher {
public:
    ContextSwitcher();
    ContextSwitcher(int popsCount, const QString& contextName, const QString& contextOperation);

    QString toString() const;
    bool isNull() const;

    void resolveContextReferences(const QHash<QString, ContextPtr>& contexts, QString& error);

protected:
    int popsCount;
    QString contextName;
    ContextPtr context;
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

    void printDescription(QTextStream& out) const;

    QString name() const;

    void resolveContextReferences(const QHash<QString, ContextPtr>& contexts, QString& error);
    void setKeywordLists(const QHash<QString, QStringList>& lists, QString& error);

protected:
    QString _name;
    QString attribute;
    ContextSwitcher lineEndContext;
    ContextSwitcher lineBeginContext;
    ContextSwitcher fallthroughContext;
    bool dynamic;
    QList<RulePtr> rules;
};
