#pragma once

#include "context.h"
#include "style.h"

class AbstractRule;


class MatchResult {
public:
    MatchResult(int length,
                void* data,
                bool lineContinue,
                const ContextSwitcher& context,
                const Style& style);
    MatchResult();

    int length;
    void* data;
    bool lineContinue;
    ContextSwitcher nextContext;
    Style style;
};
