#include "loader.h"


Context* loadContext(QXmlStreamReader& xmlReader) {
    QXmlStreamAttributes attrs = xmlReader.attributes();
    if ( ! attrs.hasAttribute("name")) {
        xmlReader.raiseError("Not found top level <language> tag");
        return nullptr;
    }

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
