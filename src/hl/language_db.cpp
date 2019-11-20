#include <QString>
#include <QMap>
#include <QRegExp>
#include <QFileInfo>

#include "qutepart.h"


namespace Qutepart {

extern QMap<QString, QString> mimeTypeToXmlFileName;
extern QMap<QString, QString> languageNameToXmlFileName;
extern QMap<QString, QString> extensionToXmlFileName;
extern QMap<QString, QString> firstLineToXmlFileName;
extern QMap<QString, QString> xmlFileNameToIndenter;

/*
 * Search value in map {glob pattern: value}
 * Match string with glob pattern key
 */
QString searchInGlobMap(const QMap<QString, QString>& map, const QString& string) {
    QMap<QString, QString>::const_iterator it = map.begin();
    while (it != map.end()) {
        QRegExp regExp(it.key(), Qt::CaseSensitive, QRegExp::Wildcard);
        if (regExp.exactMatch(string)) {
            return it.value();
        }
        ++it;
    }

    return QString::null;
}

QString chooseLanguageXmlFileName(
    const QString& mimeType,
    const QString& languageName,
    const QString& sourceFilePath,
    const QString& firstLine)
{
    if ( ! mimeType.isNull()) {
        if (mimeTypeToXmlFileName.contains(mimeType)) {
            return mimeTypeToXmlFileName[mimeType];
        }
    }

    if ( ! languageName.isNull()) {
        if (languageNameToXmlFileName.contains(languageName)) {
            return languageNameToXmlFileName[languageName];
        }
    }

    if ( ! sourceFilePath.isNull()) {
        QString fileName = QFileInfo(sourceFilePath).fileName();
        QString xmlName = searchInGlobMap(extensionToXmlFileName, fileName);
        if ( ! xmlName.isNull()) {
            return xmlName;
        }
    }

    if ( ! firstLine.isNull()) {
        QString xmlName = searchInGlobMap(firstLineToXmlFileName, firstLine);
        if ( ! xmlName.isNull()) {
            return xmlName;
        }
    }

    return QString::null;
}

/* Choose language XML file name by available parameters
 * First parameters have higher priority
 */
LangInfo chooseLanguage(const QString& mimeType,
                        const QString& languageName,
                        const QString& sourceFilePath,
                        const QString& firstLine)
{
    QString xmlName = chooseLanguageXmlFileName(mimeType, languageName, sourceFilePath, firstLine);

    if (xmlName.isNull()) {
        return LangInfo();
    } else {
        QList<QString> langNames = languageNameToXmlFileName.keys(xmlName);
        QString indenter = xmlFileNameToIndenter[xmlName];
        return LangInfo(xmlName, langNames, indenter);
    }
};

}  // namespace Qutepart
