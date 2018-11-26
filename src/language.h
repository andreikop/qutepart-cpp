#pragma once


#include <QTextStream>
#include <QList>
#include <QSharedPointer>

class Context {
public:
    Context(QString name);

    void printDescription(QTextStream& out);

protected:
    QString name;
};

typedef QSharedPointer<Context> ContextPtr;


class Language {
public:
    Language(QString name, QList<ContextPtr> contexts);

    void printDescription(QTextStream& out);

protected:
    QString name;
    QList<ContextPtr> contexts;
};
