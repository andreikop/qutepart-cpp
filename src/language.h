#pragma once


#include <QTextStream>
#include <QList>
#include <QSharedPointer>

#include "context.h"


class Language {
public:
    Language(QString name, QList<ContextPtr> contexts);

    void printDescription(QTextStream& out);

protected:
    QString name;
    QList<ContextPtr> contexts;
};
