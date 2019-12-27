#pragma once


#include <QTextStream>
#include <QList>
#include <QSet>
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
             const QSet<QString>& allLanguageKeywords,
             const QList<ContextPtr>& contexts);

    void printDescription(QTextStream& out) const;

    void highlightBlock(QTextBlock block, QVector<QTextLayout::FormatRange>& formats);

    ContextPtr defaultContext() const {return contexts.first();};
    ContextPtr getContext(const QString& name) const;

    QSet<QString> allLanguageKeywords() const;

protected:
    QString name;
    QStringList extensions;
    QStringList mimetypes;
    int priority;
    bool hidden;
    QString indenter;
    QSet<QString> allLanguageKeywords_;

    QList<ContextPtr> contexts;
    ContextStack defaultContextStack;

    ContextStack getContextStack(QTextBlock block);
    ContextStack switchAtEndOfLine(ContextStack contextStack);
};

};
