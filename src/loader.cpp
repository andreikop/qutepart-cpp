#include <QDebug>

#include "rules.h"
#include "style.h"

#include "loader.h"

const QString DEFAULT_DELIMINATOR = " \t.():!+,-<=>%&*/;?[]^{|}~\\";

QList<RulePtr> loadRules(QXmlStreamReader& xmlReader, QString& error);


QHash<QString, QString> attrsToInsensitiveHashMap(const QXmlStreamAttributes& attrs) {
    QHash<QString, QString> result;
    foreach(const QXmlStreamAttribute attr, attrs) {
        result[attr.name().toString().toLower()] = attr.value().toString();
    }

    return result;
}


QString getAttribute(QXmlStreamAttributes attrs, QString name,
                     QString defaultValue=QString::null,
                     bool warnIfNotSet=false) {
    if (attrs.hasAttribute(name)) {
        return attrs.value(name).toString();
    } else {
        if (warnIfNotSet) {
            qWarning() << "Required attribute " << name << " is not set";
        }
        return defaultValue;
    }
}

QString getRequiredAttribute(QXmlStreamAttributes attrs, QString name, QString& error) {
    if (attrs.hasAttribute(name)) {
        return attrs.value(name).toString();
    } else {
        error = QString("Required attribute %1 is not set").arg(name);
        return QString::null;
    }
}

bool parseBoolAttribute(const QString& value, QString& error) {
    QString lowerValue = value.toLower();

    if(lowerValue  == "true" || lowerValue == "1") {
        return true;
    } else if (lowerValue == "false" || lowerValue == "0") {
        return false;
    } else {
        error = QString("Invalid bool attribute value '%1'").arg(value);
        return false;
    }
}

int parseIntAttribute(const QString& value, QString& error) {
    bool intOk = true;
    int result = value.toInt(&intOk);
    if ( ! intOk) {
        error = QString("Bad integer value '%1'").arg(value);
    }

    return result;
}

QString processEscapeSequences(QString value) {
    value.replace("\\a", "\a");
    value.replace("\\b", "\b");
    value.replace("\\f", "\f");
    value.replace("\\n", "\n");
    value.replace("\\r", "\r");
    value.replace("\\t", "\t");
    value.replace("\\\\", "\\");

    return value;
}

ContextSwitcher makeContextSwitcher(QString contextOperation) {
    int popsCount = 0;
    QString contextToSwitch;

    QString rest = contextOperation;
    while(rest.startsWith("#pop")){
        popsCount += 1;
        rest = rest.remove(0, 4); // 4 is length of '#pop'
    }

    if(rest == "#stay"){
        if (popsCount > 0){
            qWarning() << QString("Invalid context operation '%1'").arg(contextOperation);
        }
    } else {
        contextToSwitch = rest;
    }

    if (popsCount > 0 || ( ! contextToSwitch.isNull())){
        return ContextSwitcher(popsCount, contextToSwitch, contextOperation);
    } else {
        return ContextSwitcher();
    }
}

AbstractRuleParams parseAbstractRuleParams(const QXmlStreamAttributes& attrs, QString& error) {
    QString attribute = getAttribute(attrs, "attribute").toLower();

    QString contextText = getAttribute(attrs, "context", "#stay");
    ContextSwitcher context = makeContextSwitcher(contextText);

    bool lookAhead = parseBoolAttribute(getAttribute(attrs, "lookAhead", "false"), error);
    if ( ! error.isNull()) {
        error = QString("Failed to parse 'lookAhead': %1").arg(error);
        return AbstractRuleParams();
    }

    bool firstNonSpace = parseBoolAttribute(getAttribute(attrs, "firstNonSpace", "false"), error);
    if ( ! error.isNull()) {
        error = QString("Failed to parse 'firstNonSpace': %1").arg(error);
        return AbstractRuleParams();
    }

    bool dynamic = parseBoolAttribute(getAttribute(attrs, "dynamic", "false"), error);
    if ( ! error.isNull()) {
        error = QString("Failed to parse 'dynamic': %1").arg(error);
        return AbstractRuleParams();
    }

    char textType = 0; // TODO

    // TODO beginRegion
    // TODO endRegion

    int column = -1;
    QString columnStr = getAttribute(attrs, "column");
    if ( ! columnStr.isNull()) {
        column = parseIntAttribute(columnStr, error);
        if ( ! error.isNull()) {
            error = QString("Bad integer column value: %1").arg(error);
            return AbstractRuleParams();
        }
    }

#if 0
    # attribute
    if attribute is not None:
        attribute = attribute.lower()  # not case sensitive
        try:
            format = attributeToFormatMap[attribute]
            textType = format.textType if format is not None else ' '
            if formatConverterFunction is not None and format is not None:
                format = formatConverterFunction(format)
        except KeyError:
            _logger.warning('Unknown rule attribute %s', attribute)
            format = parentContext.format
            textType = parentContext.textType
    else:
        format = None
        textType = None
#endif

    return AbstractRuleParams {
        textType, attribute,
        context, lookAhead,
        firstNonSpace, column,
        dynamic};
}

template<class RuleClass> RuleClass* loadStringRule(const QXmlStreamAttributes& attrs,
                                                    const AbstractRuleParams& params,
                                                    QString& error) {
    QString value = getRequiredAttribute(attrs, "String", error);
    if ( ! error.isNull()) {
        return nullptr;
    }

    bool insensitive = parseBoolAttribute(getAttribute(attrs, "insensitive", "false"), error);
    if ( ! error.isNull()) {
        error = QString("Failed to parse 'insensitive': %1").arg(error);
        return nullptr;
    }

    return new RuleClass(params, value, insensitive);
}

KeywordRule* loadKeywordRule(const QXmlStreamAttributes& attrs,
                             const AbstractRuleParams& params,
                             QString& error) {
    QString listName = getRequiredAttribute(attrs, "String", error);
    if ( ! error.isNull()) {
        return nullptr;
    }

    return new KeywordRule(params, listName);
}

DetectCharRule* loadDetectChar(const QXmlStreamAttributes& attrs,
                               const AbstractRuleParams& params,
                               QString& error) {
    QString value = getRequiredAttribute(attrs, "char", error);
    if ( ! error.isNull()) {
        return nullptr;
    }

    value = processEscapeSequences(value);

    int index = 0;

    if (params.dynamic) {
        bool ok = false;
        index = value.toInt(&ok);
        if ( ! ok) {
            error = QString("Bad integer value: %1").arg(value);
            return nullptr;
        }

        if (index < 0) {
            error = QString("Bad integer value: %1").arg(value);
            return nullptr;
        }
        value = QString::null;
    }

    return new DetectCharRule(params, value, index);
}

Detect2CharsRule* loadDetect2Chars(const QXmlStreamAttributes& attrs,
                                   const AbstractRuleParams& params,
                                   QString& error) {
    QString char0 = getRequiredAttribute(attrs, "char", error);
    if ( ! error.isNull()) {
        return nullptr;
    }
    QString char1 = getRequiredAttribute(attrs, "char1", error);
    if ( ! error.isNull()) {
        return nullptr;
    }

    QString value = processEscapeSequences(char0 + char1);

    return new Detect2CharsRule(params, value, false);
}

RegExpRule* loadRegExp(const QXmlStreamAttributes& attrs,
                       const AbstractRuleParams& params,
                       QString& error) {
    QString value = getRequiredAttribute(attrs, "String", error);
    if ( ! error.isNull()) {
        return nullptr;
    }

    bool insensitive = parseBoolAttribute(getAttribute(attrs, "insensitive", "false"), error);
    if ( ! error.isNull()) {
        error = QString("Failed to parse 'insensitive': %1").arg(error);
        return nullptr;
    }

    bool minimal = parseBoolAttribute(getAttribute(attrs, "minimal", "false"), error);
    if ( ! error.isNull()) {
        error = QString("Failed to parse 'minimal': %1").arg(error);
        return nullptr;
    }

    bool wordStart = false; // TODO
    bool lineStart = false; // TODO

    return new RegExpRule(params, value, insensitive, minimal, wordStart, lineStart);
}

template<class RuleClass>
RuleClass* loadNumberRule(QXmlStreamReader& xmlReader,
                          const AbstractRuleParams& params,
                          QString& error) {
    QList<RulePtr> children = loadRules(xmlReader, error);

    if ( ! error.isNull()) {
        error = QString("Failed to load child rules of number: %1").arg(error);
        return nullptr;
    }

    return new RuleClass(params, children);
}

RangeDetectRule* loadRangeDetectRule(const QXmlStreamAttributes& attrs,
                                     const AbstractRuleParams& params,
                                     QString& error) {
    QString char0 = getRequiredAttribute(attrs, "char", error);
    if ( ! error.isNull()) {
        return nullptr;
    }
    QString char1 = getRequiredAttribute(attrs, "char1", error);
    if ( ! error.isNull()) {
        return nullptr;
    }

    return new RangeDetectRule(params, char0, char1);
}

IncludeRulesRule* loadIncludeRulesRule(const QXmlStreamAttributes& attrs,
                                       const AbstractRuleParams& params,
                                       QString& error) {
    QString contextName = getRequiredAttribute(attrs, "context", error);
    if ( ! error.isNull()) {
        return nullptr;
    }

    return new IncludeRulesRule(params, contextName);
}

AbstractRule* loadRule(QXmlStreamReader& xmlReader, QString& error) {
    QXmlStreamAttributes attrs = xmlReader.attributes();

    AbstractRuleParams params = parseAbstractRuleParams(attrs, error);
    if ( ! error.isNull()) {
        return nullptr;
    }

    QStringRef name = xmlReader.name();

    AbstractRule* result = nullptr;
    if (name == "keyword") {
        result = loadKeywordRule(attrs, params, error);
    } else if (name == "DetectChar") {
        result = loadDetectChar(attrs, params, error);
    } else if (name == "Detect2Chars") {
        result = loadDetect2Chars(attrs, params, error);
    } else if (name == "AnyChar") {
        result = loadStringRule<AnyCharRule>(attrs, params, error);
    } else if (name == "StringDetect") {
        result = loadStringRule<StringDetectRule>(attrs, params, error);
    } else if (name == "WordDetect") {
        result = loadStringRule<WordDetectRule>(attrs, params, error);
    } else if (name == "RegExpr") {
        result = loadRegExp(attrs, params, error);
    } else if (name == "Int") {
        result = loadNumberRule<IntRule>(xmlReader, params, error);
    } else if (name == "Float") {
        result = loadNumberRule<FloatRule>(xmlReader, params, error);
    } else if (name == "HlCHex") {
        result = new HlCHexRule(params);
    } else if (name == "HlCOct") {
        result = new HlCOctRule(params);
    } else if (name == "HlCStringChar") {
        result = new HlCStringCharRule(params);
    } else if (name == "HlCChar") {
        result = new HlCCharRule(params);
    } else if (name == "RangeDetect") {
        result = loadRangeDetectRule(attrs, params, error);
    } else if (name == "LineContinue") {
        result = new LineContinueRule(params);
    } else if (name == "IncludeRules") {
        result = loadIncludeRulesRule(attrs, params, error);
    } else if (name == "DetectSpaces") {
        result = new DetectSpacesRule(params);
    } else if (name == "DetectIdentifier") {
        result = new DetectIdentifierRule(params);
    } else {
        result = new AbstractRule(/*parentContext,*/ params);
    }

    if ( ! error.isNull()) {
        return nullptr;
    }

    if ( ! xmlReader.isEndElement()) {
        xmlReader.readNextStartElement();
    }

    return result;
}

QList<RulePtr> loadRules(QXmlStreamReader& xmlReader, QString& error) {
    QList<RulePtr> rules;

    while (xmlReader.readNextStartElement()) {
        AbstractRule* rule = loadRule(xmlReader, error);
        if ( ! error.isNull()) {
            break;
        }
        rules.append(RulePtr(rule));

    }

    return rules;
}

Context* loadContext(QXmlStreamReader& xmlReader, QString& error) {
    QXmlStreamAttributes attrs = xmlReader.attributes();

    QString name = getRequiredAttribute(attrs, "name", error);
    if ( ! error.isNull()) {
        return nullptr;
    }

    QString attribute = getRequiredAttribute(attrs, "attribute", error).toLower();
    if ( ! error.isNull()) {
        return nullptr;
    }

    // TODO
    // if attribute != '<not set>':  // there are no attributes for internal contexts, used by rules. See perl.xml
    //     try:
    //         format = attributeToFormatMap[attribute]
    //     except KeyError:
    //         _logger.warning('Unknown context attribute %s', attribute)
    //         format = TextFormat()
    // else:
    //     format = None

    // textType = format.textType if format is not None else ' '
    // if formatConverterFunction is not None and format is not None:
    //     format = formatConverterFunction(format)

    QString lineEndContextText = getAttribute(attrs, "lineEndContext", "#stay");
    ContextSwitcher lineEndContext = makeContextSwitcher(lineEndContextText/*,  context.parser, formatConverterFunction*/);
    QString lineBeginContextText = getAttribute(attrs, "lineEndContext", "#stay");
    ContextSwitcher lineBeginContext = makeContextSwitcher(lineBeginContextText/*, context.parser, formatConverterFunction*/);

    bool fallthrough = parseBoolAttribute(getAttribute(attrs, "fallthrough", "false"), error);
    if ( ! error.isNull()) {
        error = QString("Failed to parse 'fallthrough': %1").arg(error);
        return nullptr;
    }

    ContextSwitcher fallthroughContext;

    if(fallthrough) {
        QString fallthroughContextText = getAttribute(attrs, "fallthroughContext", "#stay", true);
        fallthroughContext = makeContextSwitcher(fallthroughContextText/*, context.parser, formatConverterFunction*/);
    }

    bool dynamic = parseBoolAttribute(getAttribute(attrs, "dynamic", "false"), error);
    if ( ! error.isNull()) {
        error = QString("Failed to parse 'dynamic': %1").arg(error);
    }

    QList<RulePtr> rules = loadRules(xmlReader, error);
    if ( ! error.isNull()) {
        error = QString("Failed to parse context %1: %2").arg(name).arg(error);
        return nullptr;
    }

    // TODO add format, textType
    return new Context(name, attribute, lineEndContext, lineBeginContext, fallthroughContext, dynamic, rules);
}

QList<ContextPtr> loadContexts(QXmlStreamReader& xmlReader, QString& error) {
    if (xmlReader.name() != "contexts") {
        error = QString("<contexts> tag not found. Found <%1>").arg(xmlReader.name().toString());
        return QList<ContextPtr>();
    }

    QHash<QString, ContextPtr> contexts;
    while (xmlReader.readNextStartElement()) {
        if (xmlReader.name() != "context") {
            error = QString("Not expected tag when parsing contexts <%1>").arg(xmlReader.name().toString());
            return QList<ContextPtr>();
        }

        Context* ctx = loadContext(xmlReader, error);
        if (ctx == nullptr) {
            return QList<ContextPtr>();
        }

        contexts[ctx->name()] = ContextPtr(ctx);
    }

    foreach(ContextPtr ctx, contexts.values()) {
        ctx->resolveContextReferences(contexts, error);
        if ( ! error.isNull()) {
            return QList<ContextPtr>();
        }
    }

    return contexts.values();
}

QStringList loadKeywordList(QXmlStreamReader& xmlReader, QString& error) {
    QStringList list;
    while (xmlReader.readNextStartElement()) {
        if (xmlReader.name() != "item") {
            error = QString("Not expected tag <%1> when loading list").arg(xmlReader.name().toString());
            return QStringList();
        }
        list << xmlReader.readElementText();
    }

    return list;
}

QHash<QString, QStringList> loadKeywordLists(QXmlStreamReader& xmlReader, QString& error) {
    QHash<QString, QStringList> lists;

    while (xmlReader.readNextStartElement()) {
        if (xmlReader.name() != "list") {
            return lists;
        }

        QXmlStreamAttributes attrs = xmlReader.attributes();

        QString name = getRequiredAttribute(attrs, "name", error);
        if ( ! error.isNull()) {
            return QHash<QString, QStringList>();
        }

        QStringList list = loadKeywordList(xmlReader, error);
        if ( ! error.isNull()) {
            return QHash<QString, QStringList>();
        }

        lists[name] = list;
    }

    return lists;
}

QHash<QString, StylePtr> loadStyles(QXmlStreamReader& xmlReader, QString& error) {
    xmlReader.readNextStartElement();

    if (xmlReader.name() != "itemDatas") {
        error = QString("<itemDatas> tag not found. Found <%1>").arg(xmlReader.name().toString());
        return QHash<QString, StylePtr>();
    }

    QHash<QString, StylePtr> styles;

    while (xmlReader.readNextStartElement()) {
        if (xmlReader.name() != "itemData") {
            error = QString("Not expected tag when parsing itemDatas <%1>").arg(xmlReader.name().toString());
            return QHash<QString, StylePtr>();
        }

        QXmlStreamAttributes attrs = xmlReader.attributes();

        QString name = getRequiredAttribute(attrs, "name", error);
        if ( ! error.isNull()) {
            return QHash<QString, StylePtr>();
        }

        QString defStyleNum = getRequiredAttribute(attrs, "defStyleNum", error);
        if ( ! error.isNull()) {
            return QHash<QString, StylePtr>();
        }

        QString color = getAttribute(attrs, "color");
        QString selColor = getAttribute(attrs, "color");

        QHash<QString, QString> attrsMap = attrsToInsensitiveHashMap(attrs);
        QStringList flags;
        foreach(QString flagName, QStringList() << "spellChecking" << "italic" << "bold" << "underline" << "strikeout") {
            bool val = parseBoolAttribute(attrsMap.value(flagName, "false"), error);
            if ( ! error.isNull()) {
                error = QString("Failed to parse 'spellChecking' of itemData: %1").arg(error);
                return QHash<QString, StylePtr>();
            }
            if (val) {
                flags << flagName;
            }
        }

        styles[name.toLower()] = StylePtr(new Style(defStyleNum, color, selColor, flags));

        xmlReader.readNextStartElement();
    }

    return styles;
}

QSet<QChar> strToSet(const QString& str) {
    QSet<QChar> res;
    foreach(QChar chr, str) {
        res.insert(chr);
    }

    return res;
}

QString setToStr(const QSet<QChar>& set) {
    QString res;
    foreach(QChar chr, set) {
        res += chr;
    }

    return res;
}

void loadKeywordParams(const QXmlStreamAttributes& attrs,
                       QString& keywordDeliminators,
                       bool& keywordsKeySensitive,
                       QString& error) {
    if (attrs.hasAttribute("casesensitive")) {
        keywordsKeySensitive = parseBoolAttribute(getAttribute(attrs, "casesensitive"), error);
        if ( ! error.isNull()) {
            return;
        }
    }

    QSet<QChar> deliminatorSet = strToSet(keywordDeliminators);

    if (attrs.hasAttribute("weakDeliminator")) {
        QSet<QChar> weakSet = strToSet(getAttribute(attrs, "weakDeliminator"));
        deliminatorSet.subtract(weakSet);
    }

    if (attrs.hasAttribute("additionalDeliminator")) {
        QSet<QChar> additionalSet = strToSet(getAttribute(attrs, "additionalDeliminator"));
        deliminatorSet.unite(additionalSet);
    }

    keywordDeliminators = setToStr(deliminatorSet);
}

void makeKeywordsLowerCase(QHash<QString, QStringList>& keywordLists) {
    foreach(QStringList list, keywordLists) {
        for(int i = 0; i < list.size(); i++) {
            list[i] = list[i].toLower();
        }
    }
}

Language* loadLanguage(QXmlStreamReader& xmlReader, QString& error) {
    if (! xmlReader.readNextStartElement()) {
        error = "Failed to read start element";
        return nullptr;
    }

    if (xmlReader.name() != "language") {
        error = "'name' attribute not found in <language>";
        return nullptr;
    }

    QXmlStreamAttributes attrs = xmlReader.attributes();

    QString name = getRequiredAttribute(attrs, "name", error);
    if ( ! error.isNull()) {
        return nullptr;
    }

    QString extensionsStr = getRequiredAttribute(attrs, "extensions", error);
    if ( ! error.isNull()) {
        return nullptr;
    }

    QString mimetypesStr = getAttribute(attrs, "mimetypes", QString::null);
    QString priorityStr = getAttribute(attrs, "priority", QString::null);
    QString hiddenStr = getAttribute(attrs, "hidden", "false");
    QString indenter = getAttribute(attrs, "indenter", QString::null);

    int priority = 0;
    if ( ! priorityStr.isNull()) {
        priority = parseIntAttribute(priorityStr, error);
        if ( ! error.isNull()) {
            error = QString("Bad integer priority value: %1").arg(error);
            return nullptr;
        }
    }

    bool hidden = parseBoolAttribute(hiddenStr, error);
    if ( ! error.isNull()) {
        error = QString("Failed to parse 'hidden' attribute: %1").arg(error);
        return nullptr;
    }

    QStringList extensions = extensionsStr.split(';', QString::SkipEmptyParts);
    QStringList mimetypes = extensionsStr.split(';', QString::SkipEmptyParts);

    if ( ! xmlReader.readNextStartElement() ||
         xmlReader.name() != "highlighting") {
        error = "<highlighting> tag not found";
        return nullptr;
    }

    QHash<QString, QStringList> keywordLists = loadKeywordLists(xmlReader, error);
    if ( ! error.isNull()) {
        return nullptr;
    }

    QList<ContextPtr> contexts = loadContexts(xmlReader, error);
    if ( ! error.isNull()) {
        return nullptr;
    }

    QHash<QString, StylePtr> styles = loadStyles(xmlReader, error);
    if ( ! error.isNull()) {
        return nullptr;
    }

    bool keywordsKeySensitive = true;
    QString keywordDeliminators = DEFAULT_DELIMINATOR;

    while ( ! xmlReader.atEnd()) {
        xmlReader.readNextStartElement();

        if (xmlReader.name() == "keywords") {
            loadKeywordParams(xmlReader.attributes(), keywordDeliminators, keywordsKeySensitive, error);
            if ( ! error.isNull()) {
                return nullptr;
            }

            // Convert all list items to lowercase
            if ( ! keywordsKeySensitive) {
                makeKeywordsLowerCase(keywordLists);
            }
        }
    }

    foreach(ContextPtr context, contexts) {
        context->setKeywordParams(keywordLists, keywordDeliminators, keywordsKeySensitive, error);
        if ( ! error.isNull()) {
            return nullptr;
        }

        context->setStyles(styles, error);
        if ( ! error.isNull()) {
            return nullptr;
        }
    }

    Language* language = new Language(name, extensions, mimetypes,
                                      priority, hidden, indenter, contexts);
    return language;
}
