#include <QDebug>

#include "text_block_user_data.h"
#include "text_to_match.h"
#include "context_switcher.h"

#include "language.h"


Language::Language(const QString& name,
                   const QStringList& extensions,
                   const QStringList& mimetypes,
                   int priority,
                   bool hidden,
                   const QString& indenter,
                   const QList<ContextPtr>& contexts,
                   const QString& keywordDeliminators)
  : name(name),
    extensions(extensions),
    mimetypes(mimetypes),
    priority(priority),
    hidden(hidden),
    indenter(indenter),
    contexts(contexts),
    defaultContextStack(contexts[0].data()),
    keywordDeliminators(keywordDeliminators)
{
}

void Language::printDescription(QTextStream& out) const {
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

void Language::highlightBlock(QTextBlock block, QVector<QTextLayout::FormatRange>& formats) {
    QTextBlockUserData* qtData = block.userData();
    TextBlockUserData* data = nullptr;
    if (qtData != nullptr) {
        data = dynamic_cast<TextBlockUserData*>(qtData);
    }

    ContextStack contextStack = nullptr;
    if (data != nullptr) {
        contextStack = data->contexts();
    } else {
        contextStack = defaultContextStack;
    }

    TextToMatch textToMatch(block.text(), keywordDeliminators);

    QString textTypeMap(textToMatch.text.length(), ' ');

    bool lineContinue = false;

    while ( ! textToMatch.text.isEmpty()) {
        qDebug() << "In context " << contextStack.currentContext()->name();

        const Context* context = contextStack.currentContext();
        const ContextSwitcher contextSwitcher = context->parseBlock(textToMatch, formats, textTypeMap, lineContinue);
        if ( ! contextSwitcher.isNull()) {
            contextStack = contextStack.switchContext(contextSwitcher, nullptr);
        } else if ( ! textToMatch.text.isEmpty()) {
            qWarning() << "Loop detected in context " << context->name() <<
                ". parseBlock() returned but context haven't been switched and text is not empty";
                break;
        }
    }

    if ( ! lineContinue) {
        while ( ! contextStack.currentContext()->lineEndContext().isNull()) {
            ContextStack oldStack = contextStack;
            contextStack = contextStack.switchContext(contextStack.currentContext()->lineEndContext(), nullptr);
            if (oldStack == contextStack) {  // avoid infinite while loop if nothing to switch
                break;
            }
        }

        // this code is not tested, because lineBeginContext is not defined by any xml file
        if ( ! contextStack.currentContext()->lineBeginContext().isNull()) {
            contextStack = contextStack.switchContext(contextStack.currentContext()->lineBeginContext(), nullptr);
        }
    }

    block.setUserData(new TextBlockUserData(textTypeMap, contextStack));
}
