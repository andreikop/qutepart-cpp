#include "context.h"
#include "rules.h"


ContextSwitcher::ContextSwitcher()
  : popsCount(0)
{}

ContextSwitcher::ContextSwitcher(int popsCount, const QString& contextName, const QString& contextOperation)
  : popsCount(popsCount),
    contextName(contextName),
    contextOperation(contextOperation)
{}

QString ContextSwitcher::toString() const {
    return contextOperation;
}

bool ContextSwitcher::isNull() const {
    return contextOperation.isEmpty();
}

void ContextSwitcher::resolveContextReferences(const QHash<QString, ContextPtr>& contexts, QString& error) {
    if (contextName.isEmpty()) {
        return;
    }

    if (contextName.startsWith('#')) {
        return; //TODO
    }

    if ( ! contexts.contains(contextName)) {
        error = QString("Failed to get context '%1'").arg(contextName);
        return;
    }

    context = contexts[contextName];
}


Context::Context(const QString& name,
                 const QString& attribute,
                 const ContextSwitcher& lineEndContext,
                 const ContextSwitcher& lineBeginContext,
                 const ContextSwitcher& fallthroughContext,
                 bool dynamic,
                 const QList<RulePtr>& rules):
    _name(name),
    attribute(attribute),
    lineEndContext(lineEndContext),
    lineBeginContext(lineBeginContext),
    fallthroughContext(fallthroughContext),
    dynamic(dynamic),
    rules(rules)
{}

void Context::printDescription(QTextStream& out) const {
    out << "\tContext " << this->_name << "\n";
    out << "\t\tattribute: " << attribute << "\n";
    if( ! lineEndContext.isNull()) {
        out << "\t\tlineEndContext: " << lineEndContext.toString() << "\n";
    }
    if( ! lineBeginContext.isNull()) {
        out << "\t\tlineBeginContext: " << lineBeginContext.toString() << "\n";
    }
    if( ! fallthroughContext.isNull()) {
        out << "\t\tfallthroughContext: " << fallthroughContext.toString() << "\n";
    }
    if(dynamic) {
        out << "\t\tdynamic\n";
    }

    foreach(RulePtr rule, rules) {
        rule->printDescription(out);
    }
}

QString Context::name() const {
    return _name;
}

void Context::resolveContextReferences(const QHash<QString, ContextPtr>& contexts, QString& error) {
    lineEndContext.resolveContextReferences(contexts, error);
    if ( ! error.isNull()) {
        return;
    }

    lineBeginContext.resolveContextReferences(contexts, error);
    if ( ! error.isNull()) {
        return;
    }

    fallthroughContext.resolveContextReferences(contexts, error);
    if ( ! error.isNull()) {
        return;
    }

    foreach(RulePtr rule, rules) {
        rule->resolveContextReferences(contexts, error);
        if ( ! error.isNull()) {
            return;
        }
    }
}

void Context::setKeywordParams(const QHash<QString, QStringList>& lists,
                               const QString& deliminators,
                               bool caseSensitive,
                               QString& error) {
    foreach(RulePtr rule, rules) {
        rule->setKeywordParams(lists, deliminators, caseSensitive, error);
        if ( ! error.isNull()) {
            break;
        }
    }
}

void Context::setStyles(const QHash<QString, StylePtr>& styles, QString& error) {
    if ( ! attribute.isNull()) {
        if ( ! styles.contains(attribute)) {
            error = QString("Not found context '%1' attribute '%2'").arg(_name, attribute);
            return;
        }
        style = styles[attribute];
    }

    foreach(RulePtr rule, rules) {
        rule->setStyles(styles, error);
        if ( ! error.isNull()) {
            break;
        }
    }
}
