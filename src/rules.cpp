#include "rules.h"


AbstractRule::AbstractRule(/*ContextPtr parentContext,*/
                           const AbstractRuleParams& params):
    /*parentContext(parentContext),*/
    textType(params.textType),
    attribute(params.attribute),
    context(params.context),
    lookAhead(params.lookAhead),
    firstNonSpace(params.firstNonSpace),
    column(params.column),
    dynamic(params.dynamic)
{}

void AbstractRule::printDescription(QTextStream& out) const {
    out << "\t\t" << description() << "\n";
}

QString AbstractRule::description() const {
    return QString("%1(%2)").arg(name()).arg(args());
}


AbstractStringRule::AbstractStringRule(const AbstractRuleParams& params,
                                       const QString& value,
                                       bool insensitive):
    AbstractRule(params),
    value(value),
    insensitive(insensitive)
{}

QString AbstractStringRule::args() const {
    QString result = value;
    if (insensitive) {
        result += " insensitive";
    }

    return result;
}


DetectCharRule::DetectCharRule(const AbstractRuleParams& params,
                               const QString& value,
                               int index):
    AbstractRule(params),
    value(value),
    index(index)
{}

QString DetectCharRule::args() const {
    if (value.isNull()) {
        return QString("index: %1").arg(index);
    } else {
        return value;
    }
}


RegExpRule::RegExpRule(const AbstractRuleParams& params,
                       const QString& value, bool insensitive,
                       bool minimal, bool wordStart, bool lineStart):
    AbstractRule(params),
    value(value),
    insensitive(insensitive),
    minimal(minimal),
    wordStart(wordStart),
    lineStart(lineStart)
{}

QString RegExpRule::args() const {
    QString result = value;
    if (insensitive) {
        result += " insensitive";
    }
    if (minimal) {
        result += " minimal";
    }
    if (wordStart) {
        result += " wordStart";
    }
    if (lineStart) {
        result += " lineStart";
    }

    return result;
}

AbstractNumberRule::AbstractNumberRule(const AbstractRuleParams& params,
                                       const QList<RulePtr>& childRules):
    AbstractRule(params),
    childRules(childRules)
{}

void AbstractNumberRule::printDescription(QTextStream& out) const {
    AbstractRule::printDescription(out);

    foreach(RulePtr rule, childRules) {
        out << "\t\t\t" << rule->description() << "\n";
    }
}


RangeDetectRule::RangeDetectRule(const AbstractRuleParams& params, const QString& char0, const QString& char1):
    AbstractRule(params),
    char0(char0),
    char1(char1)
{}

QString RangeDetectRule::args() const {
    return QString("%1 - %2").arg(char0, char1);
}
