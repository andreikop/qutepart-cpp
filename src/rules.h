#pragma once

#include <QString>
#include <QSharedPointer>
#include <QTextStream>


#include "context.h"

class Context;
typedef QSharedPointer<Context> ContextPtr;


struct AbstractRuleParams {
    // QString format;             // may be null
    char textType;              // may be 0
    QString attribute;          // may be null
    ContextSwitcher context;
    bool lookAhead;
    bool firstNonSpace;
    int column;                 // -1 if not set
    bool dynamic;
};


class AbstractRule {
public:
    AbstractRule(/*ContextPtr parentContext,*/ const AbstractRuleParams& params);
    virtual ~AbstractRule() {};

    virtual void printDescription(QTextStream& out) const;

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


class KeywordRule: public AbstractRule {
public:
    KeywordRule (const AbstractRuleParams& params,
                 const QString& string);

    void printDescription(QTextStream& out) const override;
private:
    QString string;
};
