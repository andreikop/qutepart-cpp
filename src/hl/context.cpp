#include "context.h"
#include "rules.h"
#include "text_to_match.h"
#include "match_result.h"


namespace Qutepart {

Context::Context(const QString& name,
                 const QString& attribute,
                 const ContextSwitcher& lineEndContext,
                 const ContextSwitcher& lineBeginContext,
                 const ContextSwitcher& fallthroughContext,
                 bool dynamic,
                 const QList<RulePtr>& rules):
    _name(name),
    attribute(attribute),
    _lineEndContext(lineEndContext),
    _lineBeginContext(lineBeginContext),
    fallthroughContext(fallthroughContext),
    _dynamic(dynamic),
    rules(rules)
{}

void Context::printDescription(QTextStream& out) const {
    out << "\tContext " << this->_name << "\n";
    out << "\t\tattribute: " << attribute << "\n";
    if( ! _lineEndContext.isNull()) {
        out << "\t\tlineEndContext: " << _lineEndContext.toString() << "\n";
    }
    if( ! _lineBeginContext.isNull()) {
        out << "\t\tlineBeginContext: " << _lineBeginContext.toString() << "\n";
    }
    if( ! fallthroughContext.isNull()) {
        out << "\t\tfallthroughContext: " << fallthroughContext.toString() << "\n";
    }
    if(_dynamic) {
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
    _lineEndContext.resolveContextReferences(contexts, error);
    if ( ! error.isNull()) {
        return;
    }

    _lineBeginContext.resolveContextReferences(contexts, error);
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

void Context::setStyles(const QHash<QString, Style>& styles, QString& error) {
    if ( ! attribute.isNull()) {
        if ( ! styles.contains(attribute)) {
            error = QString("Not found context '%1' attribute '%2'").arg(_name, attribute);
            return;
        }
        style = styles[attribute];
        style.updateTextType(attribute);
    }

    foreach(RulePtr rule, rules) {
        rule->setStyles(styles, error);
        if ( ! error.isNull()) {
            break;
        }
    }
}

void appendFormat(QVector<QTextLayout::FormatRange>& formats,
                  int start,
                  int length,
                  const QTextCharFormat& format) {

    if ( (! formats.isEmpty()) &&
         (formats.last().start + formats.last().length) == start &&
         formats.last().format == format) {
        formats.last().length += length;
    } else {
        QTextLayout::FormatRange fmtRange;
        fmtRange.start = start;
        fmtRange.length = length;
        fmtRange.format = format;
        formats.append(fmtRange);
    }
}

void fillTextTypeMap(
        QString& textTypeMap,
        int start,
        int length,
        QChar textType) {
    for(int i = start; i < start + length; i++) {
        textTypeMap[i] = textType;
    }
}

// Helper function for parseBlock()
void Context::applyMatchResult(
        const TextToMatch& textToMatch,
        const MatchResult* matchRes,
        QVector<QTextLayout::FormatRange>& formats,
        QString& textTypeMap) const {
    QTextCharFormat format = matchRes->style.format();
    if ( ! format.isValid()) {
        format = this->style.format();
    }
    appendFormat(formats, textToMatch.currentColumnIndex, matchRes->length, format);

    QChar textType = matchRes->style.textType();
    if (textType == 0) {
        textType = this->style.textType();
    }
    fillTextTypeMap(textTypeMap, textToMatch.currentColumnIndex, matchRes->length, textType);
}

// Parse block. Exits, when reached end of the text, or when context is switched
const ContextSwitcher Context::parseBlock(
        TextToMatch& textToMatch,
        QVector<QTextLayout::FormatRange>& formats,
        QString& textTypeMap,
        bool& lineContinue,
        QStringList& data) const {
    while ( ! textToMatch.isEmpty()) {
        MatchResult* matchRes = tryMatch(textToMatch);

        if (matchRes != nullptr) {
            lineContinue = matchRes->lineContinue;
            applyMatchResult(textToMatch, matchRes, formats, textTypeMap);

            textToMatch.shift(matchRes->length);

            if ( ! matchRes->nextContext.isNull()) {
                ContextSwitcher result = matchRes->nextContext;
                data = matchRes->data;
                delete matchRes;
                return result;
            } else {
                delete matchRes;
            }

        } else {
            lineContinue = false;
            appendFormat(formats, textToMatch.currentColumnIndex, 1, this->style.format());
            textTypeMap[textToMatch.currentColumnIndex] = this->style.textType();

            if ( ! this->fallthroughContext.isNull()) {
                return this->fallthroughContext;
            }

            textToMatch.shiftOnce();
        }
    }

    return ContextSwitcher();
}

MatchResult* Context::tryMatch(const TextToMatch& textToMatch) const {
    foreach(RulePtr rule, rules) {
        MatchResult* matchRes = rule->tryMatch(textToMatch);
        if (matchRes != nullptr) {
            return matchRes;
        }
    }

    return nullptr;
}

};
