#include <QDebug>

#include "text_block_utils.h"
#include "indent_funcs.h"

#include "alg_lisp.h"
#include "alg_scheme.h"
#include "alg_xml.h"
#include "alg_python.h"
#include "alg_ruby.h"
#include "alg_cstyle.h"

#include "indenter.h"


namespace Qutepart {

namespace {

class IndentAlgNone: public IndentAlgImpl {
public:
    QString computeSmartIndent(QTextBlock block) const override {
        return QString::null;
    }
};


class IndentAlgNormal: public IndentAlgImpl {
public:
    QString computeSmartIndent(QTextBlock block) const override {
        return prevNonEmptyBlockIndent(block);
    }
};

}  // namespace


void IndentAlgImpl::setConfig(int width, bool useTabs) {
    width_ = width;
    useTabs_ = useTabs;
}

QString IndentAlgImpl::autoFormatLine(QTextBlock block) const {
    return computeSmartIndent(block) + stripLeftWhitespace(block.text());
}

QString IndentAlgImpl::computeSmartIndent(QTextBlock block) const {
    return "";
}


const QString& IndentAlgImpl::triggerCharacters() const {
    static const QString NULL_STRING = QString::null;
    return NULL_STRING;
}

QString IndentAlgImpl::indentText() const {
    return makeIndent(width_, useTabs_);
}

Indenter::Indenter():
    alg_(std::make_unique<IndentAlgNormal>()),
    useTabs_(false),
    width_(4)
{
    alg_->setConfig(width_, useTabs_);
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
        case INDENT_ALG_XML:
            alg_ = std::make_unique<IndentAlgXml>();
        break;
        case INDENT_ALG_SCHEME:
            alg_ = std::make_unique<IndentAlgScheme>();
        break;
        case INDENT_ALG_PYTHON:
            alg_ = std::make_unique<IndentAlgPython>();
        break;
        case INDENT_ALG_RUBY:
            alg_ = std::make_unique<IndentAlgRuby>();
        break;
        case INDENT_ALG_CSTYLE:
            alg_ = std::make_unique<IndentAlgCstyle>();
        break;
        default:
            qWarning() << "Wrong indentation algorithm requested" << alg;
        break;
    }
    alg_->setConfig(width_, useTabs_);
}

QString Indenter::indentText() const {
    return makeIndent(width_, useTabs_);
}

int Indenter::width() const {
    return width_;
}

void Indenter::setWidth(int width) {
    width_ = width;
    alg_->setConfig(width_, useTabs_);
}

bool Indenter::useTabs() const {
    return useTabs_;
}

void Indenter::setUseTabs(bool use) {
    useTabs_ = use;
    alg_->setConfig(width_, useTabs_);
}

bool Indenter::shouldAutoIndentOnEvent(QKeyEvent* event) const {
    return ( ! event->text().isEmpty() &&
            alg_->triggerCharacters().contains(event->text()));
}

bool Indenter::shouldUnindentWithBackspace(const QTextCursor& cursor) const {
    return textBeforeCursor(cursor).endsWith(indentText()) &&
           ( ! cursor.hasSelection()) &&
           (cursor.atBlockEnd() ||
            ( ! cursor.block().text()[cursor.positionInBlock() + 1].isSpace()));
}

void Indenter::indentBlock(QTextBlock block, QChar typedKey) const {
    QString prevBlockText = block.previous().text();  // invalid block returns empty text
    QString indent;
    if (typedKey == '\r' &&
        prevBlockText.trimmed().isEmpty()) {  // continue indentation, if no text
        indent = prevBlockIndent(block);

        if ( ! indent.isNull()) {
            QTextCursor cursor(block);
            cursor.insertText(indent);
        }
    } else {  // be smart
        QString indentedLine;
        if (typedKey == QChar::Null) {  // format line on shortcut
            indentedLine = alg_->autoFormatLine(block);
        } else {
            QString indent = alg_->computeSmartIndent(block);
            if ( ! indent.isNull()) {
                indentedLine = indent + stripLeftWhitespace(block.text());
            }
        }
        if ( (! indentedLine.isNull()) &&
             indentedLine != block.text()) {
            QTextCursor cursor(block);
            cursor.select(QTextCursor::LineUnderCursor);
            cursor.insertText(indentedLine);
        }
    }
}

void Indenter::onShortcutIndent(QTextCursor cursor) const {
    if (cursor.positionInBlock() == 0) {  // if no any indent - indent smartly
        QTextBlock block = cursor.block();
        QString indentedLine = alg_->computeSmartIndent(block);
        if (indentedLine.isEmpty()) {
            indentedLine = indentText();
        }
        cursor.insertText(indentedLine);
    } else {  // have some indent, insert more
        QString indent;
        if (useTabs_){
            indent = "\t";
        } else {
            int charsToInsert = width_ - (textBeforeCursor(cursor).length() % width_);
            indent.fill(' ', charsToInsert);
        }
        cursor.insertText(indent);
    }
}

void Indenter::onShortcutUnindentWithBackspace(QTextCursor& cursor) const {
    int charsToRemove = textBeforeCursor(cursor).length() % indentText().length();

    if (charsToRemove == 0) {
        charsToRemove = indentText().length();
    }

    cursor.setPosition(cursor.position() - charsToRemove, QTextCursor::KeepAnchor);
    cursor.removeSelectedText();
}

}  // namespace Qutepart
