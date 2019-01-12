#pragma once

class AbstractRule;


class MatchResult {
public:
    MatchResult(const AbstractRule* rule, int length, void* data);
    MatchResult();

    bool isMatched() const {return rule != nullptr;};

    const AbstractRule* rule;
    int length;
    void* data;
};
