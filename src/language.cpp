#include "language.h"


Language::Language(const QString& name,
                   const QStringList& extensions,
                   const QStringList& mimetypes,
                   int priority,
                   bool hidden,
                   const QString& indenter,
                   const QList<ContextPtr>& contexts)
  : name(name),
    extensions(extensions),
    mimetypes(mimetypes),
    priority(priority),
    hidden(hidden),
    indenter(indenter),
    contexts(contexts)
{}

void Language::printDescription(QTextStream& out) {
    out << "Language " << name << "\n";
    out << "\textensions: " << extensions.join(", ") << "\n";
    if ( ! mimetypes.isEmpty()) {
        out << "\tmimetypes: " << mimetypes.join(", ") << "\n";
    }
    if (priority != 0) {
        out << "\tpriority: " << priority << "\n";
    }
    if (hidden) {
        out << "\thidden";
    }
    if ( ! indenter.isNull()) {
        out << "\tindenter: " << indenter << "\n";
    }

    foreach(ContextPtr ctx, this->contexts) {
        ctx->printDescription(out);
    }
}
