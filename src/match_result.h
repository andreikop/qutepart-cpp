#pragma once

class AbstractRule;


class MatchResult {
public:
    MatchResult(const AbstractRule* rule, int length, void* data);

    const AbstractRule* rule;
    int length;
    void* data;
};
