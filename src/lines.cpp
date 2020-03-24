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

}
