#include "language.h"


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


Context::Context(QString name,
                 ContextSwitcher lineEndContext,
                 ContextSwitcher lineBeginContext,
                 ContextSwitcher fallthroughContext,
                 bool dynamic)
  : name(name),
    lineEndContext(lineEndContext),
    lineBeginContext(lineBeginContext),
    fallthroughContext(fallthroughContext),
    dynamic(dynamic)
{}


void Context::printDescription(QTextStream& out) {
    out << "\t Context " << this->name << "\n";
    // out << "\t\tattribute: " << attribute << "\n";
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


Language::Language(QString name, QList<ContextPtr> contexts)
  : name(name),
    contexts(contexts)
{}

void Language::printDescription(QTextStream& out) {
    out << "Language " << this->name << "\n";

    foreach(ContextPtr ctx, this->contexts) {
        ctx->printDescription(out);
    }
}
