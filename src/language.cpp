#include "language.h"

Context::Context(QString name)
  : name(name)
{}


void Context::printDescription(QTextStream& out) {
    out << "\t Context " << this->name << "\n";
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
