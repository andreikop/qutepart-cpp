#include "indenter.h"

#include <QDebug>

namespace Qutepart {

namespace {

int firstNonSpaceColumn(const QString& line) {
    for(int i = 0; i < line.size(); i++) {
        if ( ! line[i].isSpace()) {
            return i;
        }
    }
    return line.size();
}

QString lineIndent(const QString& line) {
    return line.left(firstNonSpaceColumn(line));
}

QString blockIndent(QTextBlock block) {
    return lineIndent(block.text());
}

QString prevBlockIndent(QTextBlock block) {
    QTextBlock prevBlock = block.previous();

    if ( ! block.isValid()) {
            return QString::null;
    }

    return blockIndent(prevBlock);
}

QTextBlock prevNonEmptyBlock(QTextBlock block) {
    if ( ! block.isValid()) {
        return QTextBlock();
    }

    block = block.previous();
    while (block.isValid() && block.text().isEmpty()) {
        block = block.previous();
    }

    return block;
}

QString prevNonEmptyBlockIndent(const QTextBlock& block) {
    return blockIndent(prevNonEmptyBlock(block));
}



class IndentAlgNone: public IndentAlg {
public:
    QString computeSmartIndent(QTextBlock block, QChar typedKey) const override {
        return QString::null;
    }
};


class IndentAlgNormal: public IndentAlg {
public:
    QString computeSmartIndent(QTextBlock block, QChar typedKey) const override {
        return prevNonEmptyBlockIndent(block);
    }
};

const QString NULL_STRING = QString::null;
}  // namespace


const QString& IndentAlg::triggerCharacters() const {
    return NULL_STRING;
}


Indenter::Indenter():
    alg(new IndentAlgNormal())
{
}

Indenter::~Indenter() {
    delete alg;
}

QString Indenter::text() const {
    return "    ";  // TODO
}

bool Indenter::shouldAutoIndentOnEvent(QKeyEvent* event) const {
    return ( ! event->text().isEmpty() &&
            alg->triggerCharacters().contains(event->text()));
}

bool Indenter::shouldUnindentWithBackspace(const QTextCursor& cursor) const {
    QString blockText = cursor.block().text();
    QStringRef textBeforeCursor = blockText.leftRef(cursor.positionInBlock());

    return textBeforeCursor.endsWith(text()) &&
           ( ! cursor.hasSelection()) &&
           (cursor.atBlockEnd() ||
            ( ! blockText[cursor.positionInBlock() + 1].isSpace()));
}

#if 0
void Indenter::autoIndentBlock(QTextBlock block, QChar typedKey) const {
    // TODO
    const QString& currentText = block.text();
    int spaceAtStartLen = firstNonSpaceColumn(currentText);
    QString indent = indentForBlock(block, typedKey);
    if (( ! indent.isNull()) &&
        indent != lineIndent(block.text())) {
            // TODO apply indent
    }
}
#endif

QString Indenter::indentForBlock(QTextBlock block, QChar typedKey) const {
    QString prevBlockText = block.previous().text();  // invalid block returns empty text
    if (typedKey == '\n' &&
        prevBlockText.trimmed().isEmpty()) {  // continue indentation, if no text
        return prevBlockIndent(block);
    } else {  // be smart
        return alg->computeSmartIndent(block, typedKey);
    }
}

void Indenter::onShortcutUnindentWithBackspace(QTextCursor& cursor) const {
    QString blockText = cursor.block().text();
    QStringRef textBeforeCursor = blockText.leftRef(cursor.positionInBlock());
    int charsToRemove = textBeforeCursor.length() % text().length();

    if (charsToRemove == 0) {
        charsToRemove = text().length();
    }

    cursor.setPosition(cursor.position() - charsToRemove, QTextCursor::KeepAnchor);
    cursor.removeSelectedText();
}

}  // namespace Qutepart
