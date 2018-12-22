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
    virtual QString name() const {return "AbstractRule";};
    virtual QString args() const {return QString::null;};

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

// A rule which has 1 string as a parameter
class AbstractStringRule: public AbstractRule {
public:
    AbstractStringRule (const AbstractRuleParams& params,
                        const QString& value);

protected:
    QString args() const override {return value;};
    QString value;
};


class KeywordRule: public AbstractRule {
public:
    KeywordRule(const AbstractRuleParams& params,
                const QString& value,
                bool insensitive);

    QString name() const override {return "Keyword";};

protected:
    QString args() const override;
    QString value;
    bool insensitive;
};


class DetectCharRule: public AbstractRule {
public:
    DetectCharRule(const AbstractRuleParams& params,
                   const QString& value,
                   int index);

    QString name() const override {return "DetectChar";};
    QString args() const override;

private:
    QString value;
    int index;
};


class Detect2CharsRule: public AbstractStringRule {
    using AbstractStringRule::AbstractStringRule;
public:

    QString name() const override {return "Detect2Chars";};
};


class AnyCharRule: public AbstractStringRule {
    using AbstractStringRule::AbstractStringRule;
public:

    QString name() const override {return "AnyChar";};
};


class StringDetectRule: public AbstractStringRule {
    using AbstractStringRule::AbstractStringRule;
public:

    QString name() const override {return "StringDetect";};
};


class WordDetectRule: public AbstractStringRule {
public:
    WordDetectRule(const AbstractRuleParams& params,
                   const QString& value,
                   bool insensitive);

    QString args();

    QString name() const override {return "WordDetect";};

private:
    QString value;
    bool insensitive;
};
