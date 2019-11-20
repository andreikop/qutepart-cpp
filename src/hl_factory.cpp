#include "hl/loader.h"
#include "hl/syntax_highlighter.h"
#include "qutepart.h"

#include "hl_factory.h"


namespace Qutepart {

namespace {

QSharedPointer<Language> chooseAndLoadLanguage(
    const QString& mimeType,
    const QString& languageName,
    const QString& sourceFilePath,
    const QString& firstLine) {
    LangInfo langInfo = chooseLanguage(mimeType, languageName, sourceFilePath, firstLine);

    if ( ! langInfo.isValid()) {
        return QSharedPointer<Language>(nullptr);
    }

    return loadLanguage(langInfo.id);
}

}

QSyntaxHighlighter* makeHighlighter(
    QObject* parent,
    const QString& languageId) {
    QSharedPointer<Language> language = loadLanguage(languageId);
    if ( ! language.isNull()) {
        return new SyntaxHighlighter(parent, language);
    }

    return nullptr;
}

QSyntaxHighlighter* makeHighlighter(
    QTextDocument* parent,
    const QString& languageId) {
    QSharedPointer<Language> language = loadLanguage(languageId);
    if ( ! language.isNull()) {
        return new SyntaxHighlighter(parent, language);
    }

    return nullptr;
}

};
