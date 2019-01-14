#include "match_result.h"

#include "rules.h"

MatchResult::MatchResult(int length,
                         void* data,
                         bool lineContinue,
                         const ContextSwitcher& context,
                         const Style& style):
    length(length),
    data(data),
    lineContinue(lineContinue),
    nextContext(context),
    style(style)
{}

MatchResult::MatchResult():
    length(0),
    data(nullptr),
    lineContinue(false)
{}
