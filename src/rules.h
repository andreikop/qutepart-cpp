#pragma once

#include <QString>
#include <QSharedPointer>


#include "context.h"

class Context;
typedef QSharedPointer<Context> ContextPtr;


class AbstractRule {
public:
    AbstractRule(/*ContextPtr parentContext,*/
                 char textType,
                 const QString& attribute,
                 const ContextSwitcher& context,
                 bool lookAhead,
                 bool firstNonSpace,
                 int column,
                 bool dynamic);

protected:
    // ContextPtr parentContext;
    QString format;             // may be null
    char textType;              // may be 0
    QString attribute;          // may be null
    ContextSwitcher context;
    bool lookAhead;
    bool firstNonSpace;
    int column;                 // -1 if not set
    bool dynamic;
};
