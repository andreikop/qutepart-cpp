#include "language.h"




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
