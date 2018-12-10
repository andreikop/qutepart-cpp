#include "context.h"


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
                 bool dynamic)
  : name(name),
    attribute(attribute),
    lineEndContext(lineEndContext),
    lineBeginContext(lineBeginContext),
    fallthroughContext(fallthroughContext),
    dynamic(dynamic)
{}


void Context::printDescription(QTextStream& out) {
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
}
