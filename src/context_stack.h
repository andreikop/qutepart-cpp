#pragma once

class ContextSwitcher;


class Context;


struct ContextStackItem {
    ContextStackItem();
    ContextStackItem(const Context* context, const void* data);

    bool operator==(const ContextStackItem& other) const;

    const Context* context;
    const void* data;
};


class ContextStack {
public:
    ContextStack(Context* context);

    bool operator==(const ContextStack& other) const;

private:
    ContextStack(const QVector<ContextStackItem>& items);

public:
    // Apply context switch operation and return new context
    ContextStack switchContext(const ContextSwitcher& operation, const void* data) const;

    // Get current context
    const Context* currentContext();

    // Get current data
    const void* currentData();

private:
    QVector<ContextStackItem> items;
};
