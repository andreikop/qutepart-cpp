#include <QDebug>

#include "context_stack.h"

// FIXME avoid data where possible


ContextStackItem::ContextStackItem():
    context(nullptr),
    data(nullptr)
{}

ContextStackItem::ContextStackItem(const Context* context, const void* data):
    context(context),
    data(data)
{}

ContextStack::ContextStack(const QVector<ContextStackItem>& items):
    items(items)
{}


ContextStack ContextStack::pop(int count) {
    if (items.size() - 1 < count) {
        qWarning() << "#pop value is too big " << items.size() << count;

        if (items.size() > 1) {
            return pop(items.size() - 1);
        } else {
            return *this;
        }
    }

    return ContextStack(items.mid(0, items.size() - 1));
}

ContextStack ContextStack::append(const Context* context, void* data) {
    auto newItems = items;
    newItems.append(ContextStackItem(context, data));
    return ContextStack(newItems);
}

const Context* ContextStack::currentContext() {
    return items.last().context;
}

const void* ContextStack::currentData() {
    return items.last().data;
}
