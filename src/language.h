#pragma once


#include <QTextStream>
#include <QList>
#include <QSharedPointer>


class Context;
typedef QSharedPointer<Context> ContextPtr;
class ContextSwitcher;
typedef QSharedPointer<ContextSwitcher> ContextSwitcherPtr;


class Context {
public:
    Context(QString name,
            ContextSwitcherPtr lineEndContext,
            ContextSwitcherPtr lineBeginContext,
            ContextSwitcherPtr fallthroughContext,
            bool dynamic);

    void printDescription(QTextStream& out);

protected:
    QString name;
    ContextSwitcherPtr lineEndContext;
    ContextSwitcherPtr lineBeginContext;
    ContextSwitcherPtr fallthroughContext;
    bool dynamic;
};


class ContextSwitcher {
public:
    ContextSwitcher(int popsCount, ContextPtr contextToSwitch, const QString& contextOperation);
    QString toString();

protected:
    int popsCount;
    ContextPtr contextToSwitch;
    QString contextOperation;

};


class Language {
public:
    Language(QString name, QList<ContextPtr> contexts);

    void printDescription(QTextStream& out);

protected:
    QString name;
    QList<ContextPtr> contexts;
};
