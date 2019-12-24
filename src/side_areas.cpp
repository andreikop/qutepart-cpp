#include <QPainter>
#include <QTextBlock>

#include "qutepart.h"

#include "side_areas.h"

namespace Qutepart {

namespace {

const int LEFT_MARGIN = 5;
const int RIGHT_MARGIN = 3;

}

LineNumberArea::LineNumberArea(Qutepart* textEdit):
    QWidget(textEdit),
    textEdit_(textEdit) {
    resize(widthHint(), height());

    connect(textEdit->document(), &QTextDocument::blockCountChanged, this, &LineNumberArea::updateWidth);
    updateWidth();
}

int LineNumberArea::widthHint() const {
    int lines = std::max(1, textEdit_->document()->blockCount());
    int digits = QString("%1").arg(lines).length();

    return LEFT_MARGIN + textEdit_->fontMetrics().width('9') * digits + RIGHT_MARGIN;
}

void LineNumberArea::updateWidth() {
    int newWidth = widthHint();
    if (newWidth != width()) {
        resize(newWidth, height());
        emit(widthChanged());
    }

    update();
}

void LineNumberArea::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    painter.fillRect(event->rect(), palette().color(QPalette::Window));
    painter.setPen(Qt::black);

    QTextBlock block = textEdit_->firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = int(textEdit_->blockBoundingGeometry(block).translated(textEdit_->contentOffset()).top());
    int bottom = top + int(textEdit_->blockBoundingRect(block).height());
    int singleBlockHeight = textEdit_->cursorRect(block, 0, 0).height();

    QRectF boundingRect = textEdit_->blockBoundingRect(block);
    int availableWidth = width() - RIGHT_MARGIN - LEFT_MARGIN;
    int availableHeight = textEdit_->fontMetrics().height();
    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            QString number = QString("%1").arg(blockNumber + 1);
            painter.drawText(LEFT_MARGIN, top,
                             availableWidth, availableHeight,
                             Qt::AlignRight, number);
            if (boundingRect.height() >= singleBlockHeight * 2) {  // wrapped block
                painter.fillRect(1, top + singleBlockHeight,
                                 width() - 2, boundingRect.height() - singleBlockHeight - 2,
                                 Qt::darkGreen);
            }
        }

        block = block.next();
        boundingRect = textEdit_->blockBoundingRect(block);
        top = bottom;
        bottom = top + int(boundingRect.height());
        blockNumber++;
    }
}

void LineNumberArea::changeEvent(QEvent* event) {
    if (event->type() == QEvent::FontChange) {
        updateWidth();
    }
}

}  // namespace Qutepart
