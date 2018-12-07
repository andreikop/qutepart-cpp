#include "loader.h"


QString safeGetRequiredAttribute(QXmlStreamAttributes attrs, QString name, QString default) {
    return getAttribute(attrs, name, default, true);
}

QString getAttribute(QXmlStreamAttributes attrs, QString name, QString default, bool required=false) {
    if (attrs.hasAttribute(name)) {
        return attrs.value(name).toString();
    } else {
        if (required) {
            qWarning() << "Required attribute '" << name << "' is not set for element '" << xmlElement.tag "'";
        }
        return default;
    }
}

bool parseBoolAttribute(const QString& value) {
    if value.lower() in ('true', '1'):
        return True
    elif value.lower() in ('false', '0'):
        return False
    else:
        raise UserWarning("Invalid bool attribute value '%s'" % value)
}


Context* loadContext(QXmlStreamReader& xmlReader) {
    QXmlStreamAttributes attrs = xmlReader.attributes();
    if ( ! attrs.hasAttribute("name")) {
        xmlReader.raiseError("Not found top level <language> tag");
        return nullptr;
    }

    QString attribute = safeGetRequiredAttribute(attrs, 'attribute', '<not set>').lower();

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

    QString lineEndContextText = getAttribute(attrs, 'lineEndContext', '#stay');
    // lineEndContext = _makeContextSwitcher(lineEndContextText,  context.parser, formatConverterFunction)
    QString lineBeginContextText = getAttribute(attrs, 'lineEndContext', '#stay');
    // lineBeginContext = _makeContextSwitcher(lineBeginContextText, context.parser, formatConverterFunction)

    if _parseBoolAttribute(xmlElement.attrib.get('fallthrough', 'false')):
        fallthroughContextText = _safeGetRequiredAttribute(xmlElement, 'fallthroughContext', '#stay')
        fallthroughContext = _makeContextSwitcher(fallthroughContextText, context.parser, formatConverterFunction)
    else:
        fallthroughContext = None

    dynamic = _parseBoolAttribute(xmlElement.attrib.get('dynamic', 'false'))

    context.setValues(attribute, format, lineEndContext, lineBeginContext, fallthroughContext, dynamic, textType)



    QStringRef name = attrs.value("name");

    xmlReader.skipCurrentElement();

    return new Context(name.toString());
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

    if ( ! xmlReader.readNextStartElement() ||
         xmlReader.name() != "contexts") {
        xmlReader.raiseError("<contexts> tag not found");
        return nullptr;
    }

    QList<ContextPtr> contexts;
    while (xmlReader.readNextStartElement()) {
        if (xmlReader.name() != "context") {
            xmlReader.raiseError(QString("Not expected tag <%1>").arg(xmlReader.name()));
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
