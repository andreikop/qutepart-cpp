#include <QDebug>

#include "text_block_user_data.h"
#include "text_to_match.h"

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
    QTextLayout::FormatRange fmt;
    fmt.start = 8;
    fmt.length = 4;
    fmt.format.setForeground(Qt::red);
    formats.append(fmt);

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

    bool lineContinue = false;
    QString textTypeMap(textToMatch.text.length(), ' ');

    while ( ! textToMatch.text.isEmpty()) {
        qDebug() << "In context " << contextStack.currentContext()->name();

#if 0
        length, newContextStack, segments, textTypeMapPart, lineContinue = \
                    contextStack.currentContext().parseBlock(contextStack, currentColumnIndex, text)

        highlightedSegments += segments
        contextStack = newContextStack
        textTypeMap += textTypeMapPart
        currentColumnIndex += length
#endif
    }

#if 0
    if ( ! lineContinue) {
        while (contextStack.currentContext().lineEndContext != nullptr) {
            oldStack = contextStack
            contextStack = contextStack.currentContext().lineEndContext.getNextContextStack(contextStack)
            if oldStack == contextStack:  # avoid infinite while loop if nothing to switch
                break
        }

        // this code is not tested, because lineBeginContext is not defined by any xml file
        if (contextStack.currentContext().lineBeginContext != nullptr):
            contextStack = contextStack.currentContext().lineBeginContext.getNextContextStack(contextStack);
    }
#endif

    block.setUserData(new TextBlockUserData(textTypeMap, contextStack));
}
