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
                        const QString& value,
                        bool insensitive);

protected:
    QString args() const override;
    QString value;
    bool insensitive;
};


class KeywordRule: public AbstractStringRule {
    using AbstractStringRule::AbstractStringRule;

public:
    QString name() const override {return "Keyword";};
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
    using AbstractStringRule::AbstractStringRule;
public:
    QString name() const override {return "WordDetect";};
};

class RegExpRule: public AbstractRule {
public:
    RegExpRule(const AbstractRuleParams& params,
               const QString& value, bool insensitive,
               bool minimal, bool wordStart, bool lineStart);

    QString name() const override {return "RegExpr";};
    QString args() const override;
private:
    QString value;
    bool insensitive;
    bool minimal;
    bool wordStart;
    bool lineStart;
};


class AbstractNumberRule: public AbstractRule {
public:
    AbstractNumberRule(const AbstractRuleParams& params,
                       const QList<RulePtr>& childRules);

protected:
   QList<RulePtr> childRules;
};

class IntRule: public AbstractNumberRule {
    using AbstractNumberRule::AbstractNumberRule;

public:
    QString name() const override {return "Int";};
};

class FloatRule: public AbstractNumberRule {
    using AbstractNumberRule::AbstractNumberRule;

public:
    QString name() const override {return "Float";};
};
