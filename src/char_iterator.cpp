#include "char_iterator.h"

namespace Qutepart {

CharIterator::CharIterator(const TextPosition& position):
        position_(position)
{}

QChar CharIterator::step() {
    if ( ! atEnd()) {
        movePosition();
        QChar retVal = position_.block.text()[position_.column];
        return retVal;
    } else {
        return QChar::Null;
    }
}

TextPosition CharIterator::currentPosition() const {
    return position_;
}

bool CharIterator::atEnd() const {
    return position_.column == 0 &&
           ( ! position_.block.isValid());
}

void ForwardCharIterator::movePosition() {
    int blockLength = position_.block.text().length();

    while (1) {
        if (position_.column < (blockLength - 1)) {
            position_.column++;
            break;
        } else {
            position_.block = position_.block.next();
            if ( ! position_.block.isValid()) {
                break;
            }

            position_.column = -1;
            /* move block backward, but the block might be empty
               Go to next while loop iteration and move back
               more blocks if necessary
             */
        }
    }
}

void BackwardCharIterator::movePosition() {
    while (1) {
        if (position_.column > 0) {
            position_.column--;
            break;
        } else {
            position_.block = position_.block.previous();
            if ( ! position_.block.isValid()) {
                break;
            }

            position_.column = position_.block.length();
            /* move block backward, but the block might be empty
               Go to next while loop iteration and move back
               more blocks if necessary
             */
        }
    }
}

}; // namespace Qutepart
