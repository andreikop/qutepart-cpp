#include <QDebug>

#include "context_switcher.h"
#include "context.h"

#include "context_stack.h"

// FIXME avoid data where possible


ContextStackItem::ContextStackItem():
    context(nullptr)
{}

ContextStackItem::ContextStackItem(const Context* context, const QStringList& data):
    context(context),
    data(data)
{}

bool ContextStackItem::operator==(const ContextStackItem& other) const {
    return context == other.context && data == other.data;
}

ContextStack::ContextStack(Context* context)
{
    items.append(ContextStackItem(context));
}

ContextStack::ContextStack(const QVector<ContextStackItem>& items):
    items(items)
{}

bool ContextStack::operator==(const ContextStack& other) const{
    return items == other.items;
}

const Context* ContextStack::currentContext() {
    return items.last().context;
}

const QStringList& ContextStack::currentData() {
    return items.last().data;
}

ContextStack ContextStack::switchContext(
        const ContextSwitcher& operation,
        const QStringList& data) const{
    auto newItems = items;

    if (operation.popsCount() > 0) {
        if (newItems.size() - 1 < operation.popsCount()) {
            qWarning() << "#pop value is too big " << newItems.size() << operation.popsCount();

            if (newItems.size() > 1) {
                newItems = newItems.mid(0, 1);
            }
        }
        newItems = newItems.mid(0, newItems.size() - operation.popsCount());
    }

    if ( ! operation.context().isNull() ) {
        QStringList dataToSave;

        if (operation.context()->dynamic()) {
            dataToSave = data;
        }

        newItems.append(ContextStackItem(operation.context().data(), data));
    }

    return ContextStack(newItems);
}
