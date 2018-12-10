#include "loader.h"

#include <QDebug>


QString getAttribute(QXmlStreamAttributes attrs, QString name, QString defaultValue, bool required=false) {
    if (attrs.hasAttribute(name)) {
        return attrs.value(name).toString();
    } else {
        if (required) {
            qWarning() << "Required attribute '" << name << "' is not set";
        }
        return defaultValue;
    }
}


QString safeGetRequiredAttribute(QXmlStreamAttributes attrs, QString name, QString defaultValue) {
    return getAttribute(attrs, name, defaultValue, true);
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


ContextSwitcher makeContextSwitcher(QString contextOperation/*, parser, formatConverterFunction*/) {
    int popsCount = 0;
    ContextPtr contextToSwitch;

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
        // contextToSwitch = _getContext(rest, parser, formatConverterFunction, None)
    }

    if (popsCount > 0 || ( ! contextToSwitch.isNull())){
        return ContextSwitcher(popsCount, contextToSwitch, contextOperation);
    } else {
        return ContextSwitcher();
    }
}


Context* loadContext(QXmlStreamReader& xmlReader) {
    QString error;

    QXmlStreamAttributes attrs = xmlReader.attributes();
    if ( ! attrs.hasAttribute("name")) {
        xmlReader.raiseError("Not found top level <language> tag");
        return nullptr;
    }

    QString attribute = safeGetRequiredAttribute(attrs, "attribute", "<not set>").toLower();

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
        xmlReader.raiseError(QString("Failed to parse 'fallthrough': %1").arg(error));
    }

    ContextSwitcher fallthroughContext;

    if(fallthrough) {
        QString fallthroughContextText = safeGetRequiredAttribute(attrs, "fallthroughContext", "#stay");
        fallthroughContext = makeContextSwitcher(fallthroughContextText/*, context.parser, formatConverterFunction*/);
    }

    bool dynamic = parseBoolAttribute(getAttribute(attrs, "dynamic", "false"), error);
    if ( ! error.isNull()) {
        xmlReader.raiseError(QString("Failed to parse 'dynamic': %1").arg(error));
    }

    // context.setValues(attribute, format, lineEndContext, lineBeginContext, fallthroughContext, dynamic, textType)

    QString name = attrs.value("name").toString();

    xmlReader.skipCurrentElement();

    // TODO add format, textType
    return new Context(name, attribute, lineEndContext, lineBeginContext, fallthroughContext, dynamic);
}

Language* loadLanguage(QXmlStreamReader& xmlReader) {
    if (! xmlReader.readNextStartElement()) {
        xmlReader.raiseError("Failed to read start element");
        return nullptr;
    }

    if (xmlReader.name() != "language") {
        xmlReader.raiseError("'name' attribute not found in <language>");
        return nullptr;
    }

    QXmlStreamAttributes attrs = xmlReader.attributes();
    if ( ! attrs.hasAttribute("name")) {
        xmlReader.raiseError("Not found top level <language> tag");
        return nullptr;
    }

    QStringRef name = attrs.value("name");

    if ( ! xmlReader.readNextStartElement() ||
         xmlReader.name() != "highlighting") {
        xmlReader.raiseError("<highlighting> tag not found");
        return nullptr;
    }

    while (xmlReader.readNextStartElement()) {
        if (xmlReader.name() == "list") {
            xmlReader.skipCurrentElement(); // TODO load list
        } else {
            break;
        }
    }

    if (xmlReader.name() != "contexts") {
        xmlReader.raiseError(QString("<contexts> tag not found. Found <%1>").arg(xmlReader.name().toString()));
        return nullptr;
    }

    QList<ContextPtr> contexts;
    while (xmlReader.readNextStartElement()) {
        if (xmlReader.name() != "context") {
            xmlReader.raiseError(QString("Not expected tag <%1>").arg(xmlReader.name().toString()));
            return nullptr;
        }

        Context* ctx = loadContext(xmlReader);
        if (ctx == nullptr) {
            return nullptr;
        }
        contexts.append(ContextPtr(ctx));
    }

    Language* language = new Language(name.toString(), contexts);
    return language;
}
