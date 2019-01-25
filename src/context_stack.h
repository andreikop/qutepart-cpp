#pragma once

#include <QStringList>


class ContextSwitcher;


class Context;


struct ContextStackItem {
    ContextStackItem();
    ContextStackItem(const Context* context, const QStringList& data=QStringList());

    bool operator==(const ContextStackItem& other) const;

    const Context* context;
    QStringList data;
};


class ContextStack {
public:
    ContextStack(Context* context);

    bool operator==(const ContextStack& other) const;

private:
    ContextStack(const QVector<ContextStackItem>& items);

public:
    // Apply context switch operation and return new context
    ContextStack switchContext(
        const ContextSwitcher& operation,
        const QStringList& data=QStringList()) const;

    // Get current context
    const Context* currentContext();

    // Get current data
    const QStringList& currentData();

private:
    QVector<ContextStackItem> items;
};
