#include <QString>


namespace Qutepart {

/* Choose language XML file name by available parameters
 * First parameters have higher priority
 * Returns QString::null if failed
 */
QString chooseLanguage(const QString& mimeType = QString::null,
                       const QString& languageName = QString::null,
                       const QString& sourceFilePath = QString::null,
                       const QString& firstLine = QString::null);

};
