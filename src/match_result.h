#pragma once

#include "context.h"
#include "style.h"

class AbstractRule;


class MatchResult {
public:
    MatchResult(const AbstractRule* rule, int length, void* data);
    MatchResult();

    int length;
    void* data;
    bool lineContinue;
    ContextSwitcher nextContext;
    Style style;
};
