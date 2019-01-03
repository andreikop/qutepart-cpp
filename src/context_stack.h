#pragma once


class Context;


struct ContextStackItem {
    ContextStackItem();
    ContextStackItem(const Context* context, const void* data);

    const Context* context;
    const void* data;
};


class ContextStack {
private:
    ContextStack(const QVector<ContextStackItem>& items);

public:
    // Returns new context stack, which doesn't contain few levels
    ContextStack pop(int count);

    // Returns new context, which contains current stack and new frame
    ContextStack append(const Context* context, void* data);

    // Get current context
    const Context* currentContext();

    // Get current data
    const void* currentData();

private:
    QVector<ContextStackItem> items;
};
