#include <QPainter>
#include <QTextBlock>
#include <QIcon>
#include <QDebug>

#include "qutepart.h"
#include "text_block_flags.h"

#include "side_areas.h"

namespace Qutepart {

namespace {

const int LEFT_LINE_NUM_MARGIN = 5;
const int RIGHT_LINE_NUM_MARGIN = 3;

const int MARK_MARGIN = 1;

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

    return LEFT_LINE_NUM_MARGIN + textEdit_->fontMetrics().width('9') * digits + RIGHT_LINE_NUM_MARGIN;
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
    int availableWidth = width() - RIGHT_LINE_NUM_MARGIN - LEFT_LINE_NUM_MARGIN;
    int availableHeight = textEdit_->fontMetrics().height();
    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            QString number = QString("%1").arg(blockNumber + 1);
            painter.drawText(LEFT_LINE_NUM_MARGIN, top,
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


MarkArea::MarkArea(Qutepart* qpart):
    QWidget(qpart),
    qpart_(qpart) {
#if 0
    setMouseTracking(true);
#endif

    bookmarkPixmap_ = loadIcon("emblem-favorite");
    // self._lintPixmaps = {qpart.LINT_ERROR: self._loadIcon('emblem-error'),
    //                      qpart.LINT_WARNING: self._loadIcon('emblem-warning'),
    //                      qpart.LINT_NOTE: self._loadIcon('emblem-information')}
}

QPixmap MarkArea::loadIcon(const QString& name) const {
    QIcon icon = QIcon::fromTheme(name);
    int size = qpart_->cursorRect(qpart_->document()->begin(), 0, 0).height() - 6;
    return icon.pixmap(size, size);  // This also works with Qt.AA_UseHighDpiPixmaps
}

int MarkArea::widthHint() const {
    return MARK_MARGIN + bookmarkPixmap_.width() + MARK_MARGIN;
}

void MarkArea::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    painter.fillRect(event->rect(), palette().color(QPalette::Window));

    QTextBlock block = qpart_->firstVisibleBlock();
    QRectF blockBoundingGeometry = qpart_->blockBoundingGeometry(block).translated(qpart_->contentOffset());
    int top = blockBoundingGeometry.top();
    int bottom = top + blockBoundingGeometry.height();

    while (block.isValid() && top <= event->rect().bottom()) {
        int height = qpart_->blockBoundingGeometry(block).height();

        if (block.isVisible() && bottom >= event->rect().top()) {
#if 0
            if block.blockNumber() in self._qpart.lintMarks:
                msgType, msgText = self._qpart.lintMarks[block.blockNumber()]
                pixMap = self._lintPixmaps[msgType]
                yPos = top + ((height - pixMap.height()) / 2)  # centered
                painter.drawPixmap(0, yPos, pixMap)
#endif

            if (isBookmarked(block)) {
                int yPos = top + ((height - bookmarkPixmap_.height()) / 2);  // centered
                painter.drawPixmap(0, yPos, bookmarkPixmap_);
            }
        }

        top += height;
        block = block.next();
    }
}

#if 0  // TODO linter marks
void MarkArea::mouseMoveEvent(QMouseEvent* event) {
    blockNumber = self._qpart.cursorForPosition(event.pos()).blockNumber()
    if blockNumber in self._qpart._lintMarks:
        msgType, msgText = self._qpart._lintMarks[blockNumber]
        QToolTip.showText(event.globalPos(), msgText)
    else:
        QToolTip.hideText()

    return QWidget::mouseMoveEvent(event);
}
#endif

}  // namespace Qutepart
