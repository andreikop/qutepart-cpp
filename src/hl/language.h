#pragma once


#include <QTextStream>
#include <QList>
#include <QTextBlock>

#include "context.h"
#include "context_stack.h"

namespace Qutepart {

class Language {
public:
    Language(const QString& name,
             const QStringList& extensions,
             const QStringList& mimetypes,
             int priority,
             bool hidden,
             const QString& indenter,
             const QList<ContextPtr>& contexts,
             const QString& keywordDeliminators);

    void printDescription(QTextStream& out) const;

    void highlightBlock(QTextBlock block, QVector<QTextLayout::FormatRange>& formats);

    ContextPtr defaultContext() const {return contexts.first();};
    ContextPtr getContext(const QString& name) const;

protected:
    QString name;
    QStringList extensions;
    QStringList mimetypes;
    int priority;
    bool hidden;
    QString indenter;

    QList<ContextPtr> contexts;
    ContextStack defaultContextStack;
    QString keywordDeliminators;
};

};
