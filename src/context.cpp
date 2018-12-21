#include "context.h"
#include "rules.h"


ContextSwitcher::ContextSwitcher()
  : popsCount(0)
{}

ContextSwitcher::ContextSwitcher(int popsCount, ContextPtr contextToSwitch, const QString& contextOperation)
  : popsCount(popsCount),
    contextToSwitch(contextToSwitch),
    contextOperation(contextOperation)
{}

QString ContextSwitcher::toString() const {
    return contextOperation;
}

bool ContextSwitcher::isNull() const {
    return contextOperation.isEmpty();
}


Context::Context(const QString& name,
                 const QString& attribute,
                 const ContextSwitcher& lineEndContext,
                 const ContextSwitcher& lineBeginContext,
                 const ContextSwitcher& fallthroughContext,
                 bool dynamic,
                 const QList<RulePtr>& rules)
  : name(name),
    attribute(attribute),
    lineEndContext(lineEndContext),
    lineBeginContext(lineBeginContext),
    fallthroughContext(fallthroughContext),
    dynamic(dynamic),
    rules(rules)
{}


void Context::printDescription(QTextStream& out) const {
    out << "\tContext " << this->name << "\n";
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
        out << "\t\t" << rule->description() << "\n";
    }
}
