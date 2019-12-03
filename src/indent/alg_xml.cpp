#include <QDebug>
#include <QRegularExpression>

#include "text_block_utils.h"

#include "alg_xml.h"


namespace Qutepart {

const QString& IndentAlgXml::triggerCharacters() const {
    static QString chars = "/>";
    return chars;
}

namespace {
bool matches(const QString& pattern, const QString& text) {
    return QRegularExpression(pattern).match(text).capturedLength() > 0;
}
}


QString IndentAlgXml::computeSmartIndent(QTextBlock block, const QString& configuredIndent, QChar typedKey) const {
    QString lineText = block.text();

    QString prevLineText = prevNonEmptyBlock(block).text();

    bool alignOnly = (typedKey == QChar::Null);

    if (alignOnly) {
#if 0
        QRegularExpression splitter(">\\s*<");
        QString lineToSplit = lineText.left(lineText.length());

        auto match = splitter.match(lineToSplit);

        if (match.capturedLength() > 0) {
            while (match.capturedLength() > 0) {
                QString newLine = lineToSplit.left(match.capturedStart() + 1);  // +1 for >
                lineToSplit = lineToSplit.mid(match.capturedEnd() - 1);  // -1 for <

                QChar ch = '\n';
                if (matches("^\\s*</", newLine)) {
                    ch = '/';
                } else if (matches("\\>[^<>]*$", newLine)) {
                    ch = '>';
                }

                // split line
                QTextCursor cursor(block);
                setPositionInBlock(&cursor, match.capturedStart() + 1);
                cursor.insertBlock();

                // Indent line
                QString indent = processChar(newLine, prevLineText, ch, configuredIndent);
                cursor = QTextCursor(block);
                setBlockIndent(&cursor, indent);

                prevLineText = cursor.block().text();
                match = splitter.match(lineToSplit);
            }
        } else {  // no tokens, do not split line, just compute indent
#endif
            QChar ch = '\n';
            if (matches("^\\s*</", lineText)) {
                ch = '/';
            } else if (matches(">[^<>]*", lineText)) {
                ch = '>';
            }
            return processChar(lineText, prevLineText, ch, configuredIndent);
#if 0
        }
#endif
    }

    return processChar(lineText, prevLineText, typedKey, configuredIndent);
}

QString IndentAlgXml::processChar(
        const QString& lineText,
        const QString& prevLineText,
        QChar ch,
        const QString& configuredIndent) const {
    QString prevIndent = lineIndent(prevLineText);

    if (ch == '/') {
        if ( ! matches("^\\s*</", lineText)) {
            /* might happen when we have something like <foo bar="asdf/ycxv">
             * don't change indentation then
             */
            return prevIndent;
        }

        if ( ! matches("\\s*<[^/][^>]*[^/]>[^<>]*$", prevLineText)) {
            // decrease indent when we write </ and prior line did not start a tag
            return decreaseIndent(prevIndent, configuredIndent);
        }
    } else if (ch == '>') {
        /* increase indent width when we write <...> or <.../> but not </...>
         * and the prior line didn't close a tag
         */
        if (prevLineText.isEmpty()) { // first line, zero indent
            return "";
        }

        if (matches("^<(\\?xml|!DOCTYPE).*", prevLineText)) {
            return "";
        } else if (matches("^<(\?xml|!DOCTYPE).*", lineText)) {
            return "";
        } else if (matches("^\\s*</", lineText)) {
            // closing tag, decrease indentation when previous didn't open a tag
            if (matches("\\s*<[^/][^>]*[^/]>[^<>]*$", prevLineText)) {
                // keep indent when prev line opened a tag
                return prevIndent;
            } else {
                return decreaseIndent(prevIndent, configuredIndent);
            }
        }
        else if (matches("<([/!][^>]+|[^>]+/)>\\s*$", prevLineText)) {
            // keep indent when prev line closed a tag or was empty or a comment
            return prevIndent;
        }

        return increaseIndent(prevIndent, configuredIndent);
    } else if (ch == '\n' || ch == '\r') {
        if (matches("^<(\\?xml|!DOCTYPE)", prevLineText)) {
            return "";
        } else if (matches("<([^/!]|[^/!][^>]*[^/])>[^<>]*$", prevLineText)) {
            // increase indent when prev line opened a tag (but not for comments)
            return increaseIndent(prevIndent, configuredIndent);
        }
    }

    return prevIndent;
}

};  // namespace Qutepart
