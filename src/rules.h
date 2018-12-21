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

    virtual QString description() const;

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

    QString description() const override;

private:
    QString string;
};


class DetectCharRule: public AbstractRule {
public:
    DetectCharRule(const AbstractRuleParams& params,
                   const QString& value,
                   int index);

    QString description() const override;

private:
    QString value;
    int index;
};


class Detect2CharsRule: public AbstractRule {
public:
    Detect2CharsRule(const AbstractRuleParams& params,
                     const QString& value);

    QString description() const override;

private:
    QString value;
};
