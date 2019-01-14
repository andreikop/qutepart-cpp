#include "match_result.h"

#include "rules.h"

MatchResult::MatchResult(const AbstractRule* rule, int length, void* data):
    length(rule->lookAhead ? 0 : length),
    data(data),
    lineContinue(false)
{}

MatchResult::MatchResult():
    length(0),
    data(nullptr),
    lineContinue(false)
{}
