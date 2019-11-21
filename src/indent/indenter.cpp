#include <QDebug>

#include "text_block_utils.h"

#include "alg_lisp.h"

#include "indenter.h"


namespace Qutepart {

namespace {

class IndentAlgNone: public IndentAlgImpl {
public:
    QString computeSmartIndent(QTextBlock block, QChar typedKey) const override {
        return QString::null;
    }
};


class IndentAlgNormal: public IndentAlgImpl {
public:
    QString computeSmartIndent(QTextBlock block, QChar typedKey) const override {
        return prevNonEmptyBlockIndent(block);
    }
};

const QString NULL_STRING = QString::null;
}  // namespace


const QString& IndentAlgImpl::triggerCharacters() const {
    return NULL_STRING;
}


Indenter::Indenter():
    alg_(std::make_unique<IndentAlgNormal>()),
    useTabs_(false),
    width_(4)
{
}

void Indenter::setAlgorithm(IndentAlg alg) {
    switch (alg) {
        case INDENT_ALG_NONE:
            alg_ = std::make_unique<IndentAlgNone>();
        break;
        case INDENT_ALG_NORMAL:
            alg_ = std::make_unique<IndentAlgNormal>();
        break;
        case INDENT_ALG_LISP:
            alg_ = std::make_unique<IndentAlgLisp>();
        break;
        default:
            qWarning() << "Wrong indentation algorithm requested" << alg;
        break;
    }
}

QString Indenter::text() const {
    if (useTabs_){
        return "\t";
    } else {
        return QString().fill(' ', 4);
    }
}

int Indenter::width() const {
    return width_;
}

void Indenter::setWidth(int width) {
    width_ = width;
}

bool Indenter::useTabs() const {
    return useTabs_;
}

void Indenter::setUseTabs(bool use) {
    useTabs_ = use;
}

bool Indenter::shouldAutoIndentOnEvent(QKeyEvent* event) const {
    return ( ! event->text().isEmpty() &&
            alg_->triggerCharacters().contains(event->text()));
}

bool Indenter::shouldUnindentWithBackspace(const QTextCursor& cursor) const {
    return textBeforeCursor(cursor).endsWith(text()) &&
           ( ! cursor.hasSelection()) &&
           (cursor.atBlockEnd() ||
            ( ! cursor.block().text()[cursor.positionInBlock() + 1].isSpace()));
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
    if (typedKey == '\r' &&
        prevBlockText.trimmed().isEmpty()) {  // continue indentation, if no text
        return prevBlockIndent(block);
    } else {  // be smart
        return alg_->computeSmartIndent(block, typedKey);
    }
}

void Indenter::onShortcutIndent(QTextCursor cursor) const {
    QString indent;
    if (cursor.positionInBlock() == 0) {  // if no any indent - indent smartly
        QTextBlock block = cursor.block();
        indent = alg_->computeSmartIndent(block);
        if (indent.isEmpty()) {
            indent = text();
        }
    } else {  // have some indent, insert more
        if (useTabs_){
            indent = "\t";
        } else {
            int charsToInsert = width_ - (textBeforeCursor(cursor).length() % width_);
            indent.fill(' ', charsToInsert);
        }
    }

    cursor.insertText(indent);
}

void Indenter::onShortcutUnindentWithBackspace(QTextCursor& cursor) const {
    int charsToRemove = textBeforeCursor(cursor).length() % text().length();

    if (charsToRemove == 0) {
        charsToRemove = text().length();
    }

    cursor.setPosition(cursor.position() - charsToRemove, QTextCursor::KeepAnchor);
    cursor.removeSelectedText();
}

}  // namespace Qutepart
