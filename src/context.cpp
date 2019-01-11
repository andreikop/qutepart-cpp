#include "context.h"
#include "rules.h"
#include "text_to_match.h"



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
        fmt.start = start;
        fmt.length = length;
        fmt.format = format;
        formats.append(fmt);
    }
}

void fillTexTypeMap(QString& textTypeMap,
                    int start,
                    int length,
                    QChar textType) {
    for(int i = start; i < start + length; i++) {
        textTypeMap[i] = textType;
    }
}

// Helper function for parseBlock()
void Context::applyMatchResult(TextToMatch& textToMatch, MatchResult& matchRes,
                      QVector<QTextLayout::FormatRange>& formats,
                      QString& textTypeMap) {
    QTextCharFormat format = matchRes.rule->format;
    if ( ! format.isValid()) {
        format = this->format;
    }
    appendFormat(formats, textToMatch.currentColumnIndex, matchRes.length, format);

    QChar textType = matchRes.rule->textType;
    if (textType == 0) {
        textType = this->textType;
    }
    fillTextTypeMap(textTypeMap, textToMatch.currentColumnIndex, matchRes.length, textType);
}

// Parse block. Exits, when reached end of the text, or when context is switched
ContextSwitcher* Context::parseBlock(TextToMatch& textToMatch,
                                     QVector<QTextLayout::FormatRange>& formats,
                                     QString& textTypeMap,
                                     bool& lineContinue) const {
    QTextCharFormat format;
    format.setForeground(Qt::red);
    appendFormat(formats, 4, 8, format);

    while ( ! textToMatch.isEmpty()) {
        MatchResult matchRes;
        foreach(RulePtr rule, rules) {
            matchRes = rule->tryMatch(textToMatch);
            if (matchRes.isMatched()) {
                break;
            }
        }

        if (matchRes.isMatched()) {
            lineContinue = dynamic_cast<LineContinue*>(matchRes.rule) != nullptr;

            applyMatchResult(textToMatch, matchRes, formats, textTypeMap);

            if ( ! matchRes.rule->context.isNull()) {
                return matchRes.rule->context; // TODO return data
            }

            textToMatch.shift(matchRes.length);
        } else {
            lineContinue = false;

            appendFormat(formats, textToMatch.currentColumnIndex, 1, this->format);
            textTypeMap[textToMatch.currentColumnIndex] = this->textType;

            if ( ! this->fallthroughContext->isNull()) {
                return this->fallthroughContext;
            }

            textToMatch.shiftOnce();
        }
    }

    return nullptr;
}
