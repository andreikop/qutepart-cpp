#include "rules.h"

AbstractRule::AbstractRule(/*ContextPtr parentContext,*/
                           char textType,
                           const QString& attribute,
                           const ContextSwitcher& context,
                           bool lookAhead,
                           bool firstNonSpace,
                           int column,
                           bool dynamic)
  : /*parentContext(parentContext),*/
    textType(textType),
    attribute(attribute),
    context(context),
    lookAhead(lookAhead),
    firstNonSpace(firstNonSpace),
    column(column),
    dynamic(dynamic)
{}
