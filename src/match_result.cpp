#include "match_result.h"

#include "rules.h"

MatchResult::MatchResult(const AbstractRule* rule, int length, void* data):
    rule(rule),
    length(rule->lookAhead ? 0 : length),
    data(data)
{}

MatchResult::MatchResult():
    rule(nullptr),
    length(0),
    data(nullptr)
{}
