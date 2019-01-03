#pragma once

#include <QString>
#include <QHash>
#include <QSharedPointer>


class Context;
typedef QSharedPointer<Context> ContextPtr;


class ContextSwitcher {
public:
    ContextSwitcher();
    ContextSwitcher(int popsCount, const QString& contextName, const QString& contextOperation);

    QString toString() const;
    bool isNull() const;

    void resolveContextReferences(const QHash<QString, ContextPtr>& contexts, QString& error);

    int popsCount() const {return _popsCount;};
    ContextPtr context() const {return _context;};

protected:
    int _popsCount;
    QString contextName;
    ContextPtr _context;
    QString contextOperation;
};
