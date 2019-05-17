#include "hl/language_db.h"
#include "hl/loader.h"
#include "hl/syntax_highlighter.h"

#include "hl_factory.h"


namespace Qutepart {

namespace {

QSharedPointer<Language> chooseAndLoadLanguage(
    const QString& mimeType,
    const QString& languageName,
    const QString& sourceFilePath,
    const QString& firstLine) {
    QString xmlFileName = chooseLanguage(mimeType, languageName, sourceFilePath, firstLine);

    if (xmlFileName.isNull()) {
        return QSharedPointer<Language>(nullptr);
    }

    return loadLanguage(xmlFileName);
}

}

QSyntaxHighlighter* makeHighlighter(
    QObject* parent,
    const QString& mimeType,
    const QString& languageName,
    const QString& sourceFilePath,
    const QString& firstLine) {
    QSharedPointer<Language> language = chooseAndLoadLanguage(mimeType, languageName, sourceFilePath, firstLine);
    if ( ! language.isNull()) {
        return new SyntaxHighlighter(parent, language);
    }

    return nullptr;
}

QSyntaxHighlighter* makeHighlighter(
    QTextDocument* parent,
    const QString& mimeType,
    const QString& languageName,
    const QString& sourceFilePath,
    const QString& firstLine) {
    QSharedPointer<Language> language = chooseAndLoadLanguage(mimeType, languageName, sourceFilePath, firstLine);
    if ( ! language.isNull()) {
        return new SyntaxHighlighter(parent, language);
    }

    return nullptr;
}

};
