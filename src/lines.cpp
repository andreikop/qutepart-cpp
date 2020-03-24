/* Lines API implementation.
The declarations (header) are in qutepart.h
*/

#include "qutepart.h"


namespace Qutepart {

Line::Line(const QTextBlock& block):
    block_(block)
{}

QString Line::text() const {
	return block_.text();
}

int Line::length() const {
    return block_.length() - 1;
}

void Line::remove(int pos, int count) {
    int blockLen = block_.length();

    if (pos < 0 || pos > blockLen) {
        qFatal("Wrong Line::remove(pos) %d", pos);
    }

    if (count < 0 || pos + count > blockLen) {
        qFatal("Wrong Line::remove(count) %d", count);
    }

    QTextCursor cursor(block_);
    cursor.setPosition(block_.position() + pos);
    cursor.setPosition(block_.position() + pos + count, QTextCursor::KeepAnchor);
    cursor.removeSelectedText();
}


LineIterator::LineIterator(const QTextBlock& block):
    block_(block)
{}

bool LineIterator::operator!=(const LineIterator& other) {
    if (block_.isValid()) {
        return block_ != other.block_;
    } else {
        return other.block_.isValid();
    }
}

bool LineIterator::operator==(const LineIterator& other) {
    if (block_.isValid()) {
        return block_ == other.block_;
    } else {
        return ( ! other.block_.isValid());
    }
}

LineIterator LineIterator::operator++() {
    LineIterator retVal = *this;
    block_ = block_.next();
    return retVal;
}

Line LineIterator::operator*() {
    return Line(block_);
}

Lines::Lines(QTextDocument* document):
    document_(document)
{}

LineIterator Lines::begin() {
    return LineIterator(document_->firstBlock());
}

LineIterator Lines::end() {
    return LineIterator(QTextBlock());
}

void Lines::append(const QString& lineText) {
    QTextCursor cursor(document_->lastBlock());
    cursor.movePosition(QTextCursor::End);

    cursor.beginEditBlock();

    cursor.insertBlock();
    cursor.insertText(lineText);

    cursor.endEditBlock();
}

}
