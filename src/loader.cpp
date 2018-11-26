#include "loader.h"


Language* loadLanguage(QXmlStreamReader& xmlReader) {
    bool ok = xmlReader.readNextStartElement();
    if (! ok) {
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
    Language* language = new Language(name.toString());

    return language;
}
