#pragma once


#include <QTextStream>
#include <QList>
#include <QSharedPointer>


class Context;
typedef QSharedPointer<Context> ContextPtr;
class ContextSwitcher;


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
            bool dynamic);

    void printDescription(QTextStream& out);

protected:
    QString name;
    QString attribute;
    ContextSwitcher lineEndContext;
    ContextSwitcher lineBeginContext;
    ContextSwitcher fallthroughContext;
    bool dynamic;
};


class Language {
public:
    Language(QString name, QList<ContextPtr> contexts);

    void printDescription(QTextStream& out);

protected:
    QString name;
    QList<ContextPtr> contexts;
};
