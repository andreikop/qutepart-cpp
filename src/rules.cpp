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

QString AbstractRule::description() const {
    return "AbstractRule";
}


KeywordRule::KeywordRule(const AbstractRuleParams& params,
                         const QString& string):
    AbstractRule(params),
    string(string)
{}

QString KeywordRule::description() const {
    return QString("Keyword(%1)").arg(string);
}

DetectCharRule::DetectCharRule(const AbstractRuleParams& params,
                               const QString& value,
                               int index):
    AbstractRule(params),
    value(value),
    index(index)
{}

QString DetectCharRule::description() const {
    QString indexStr = value.isNull() ? QString(" index: %1").arg(index) : "";
    return QString("Keyword(%1%2)").arg(value).arg(indexStr);
}
