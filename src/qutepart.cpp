#include <QAction>
#include <QPainter>
#include <QDebug>

#include "qutepart.h"
#include "hl_factory.h"
#include "bracket_highlighter.h"
#include "side_areas.h"
#include "indent/indenter.h"

#include "hl/loader.h"
#include "hl/syntax_highlighter.h"

#include "text_block_utils.h"
#include "text_block_flags.h"


namespace Qutepart {

class EdgeLine: public QWidget {
public:
    EdgeLine(Qutepart* qpart):
        QWidget(qpart),
        qpart(qpart)
    {
        setAttribute(Qt::WA_TransparentForMouseEvents);
    }

    void paintEvent(QPaintEvent* event) {
        QPainter painter(this);
        painter.fillRect(event->rect(), qpart->lineLengthEdgeColor());
    }

private:
    Qutepart* qpart;
};


Qutepart::Qutepart(QWidget *parent, const QString& text):
    QPlainTextEdit(text, parent),
    indenter_(std::make_unique<Indenter>()),
    markArea_(std::make_unique<MarkArea>(this)),
    drawIndentations_(true),
    drawAnyWhitespace_(false),
    drawIncorrectIndentation_(true),
    drawSolidEdge_(true),
    lineLengthEdge_(80),
    lineLengthEdgeColor_(Qt::red),
    solidEdgeLine_(new EdgeLine(this)),
    totalMarginWidth_(0)
{
    initActions();
    setAttribute(Qt::WA_KeyCompression, false);  // vim can't process compressed keys

    setDrawSolidEdge(drawSolidEdge_);
    updateTabStopWidth();
    connect(this, &Qutepart::cursorPositionChanged, this, &Qutepart::updateExtraSelections);

    setBracketHighlightingEnabled(true);
    setLineNumbersVisible(true);
}

Qutepart::~Qutepart() {
}

void Qutepart::setFont(const QFont& font) {
    // Set font and update tab stop width
    QPlainTextEdit::setFont(font);
    updateTabStopWidth();

    /* text on line numbers may overlap, if font is bigger, than code font
       Note: the line numbers margin recalculates its width and if it has
           been changed then it calls updateViewport() which in turn will
           update the solid edge line geometry. So there is no need of an
           explicit call self._setSolidEdgeGeometry() here.
     */
#if 0  // TODO
    lineNumbersMargin = self.getMargin("line_numbers")
    if lineNumbersMargin:
        lineNumbersMargin.setFont(font)
#endif
}

void Qutepart::setHighlighter(const QString& languageId) {
    highlighter_ = QSharedPointer<QSyntaxHighlighter>(makeHighlighter(document(), languageId));
    indenter_->setLanguage(languageId);
}

void Qutepart::setIndentAlgorithm(IndentAlg indentAlg) {
    indenter_->setAlgorithm(indentAlg);
}

void Qutepart::autoIndentCurrentLine() {
    QTextCursor cursor = textCursor();
    indenter_->indentBlock(cursor.block(), 0, QChar::Null);
}

bool Qutepart::indentUseTabs() const {
    return indenter_->useTabs();
}

void Qutepart::setIndentUseTabs(bool use) {
    indenter_->setUseTabs(use);
}

int Qutepart::indentWidth() const {
    return indenter_->width();
}

void Qutepart::setIndentWidth(int width) {
    indenter_->setWidth(width);
    updateTabStopWidth();
}

bool Qutepart::drawIndentations() const {
    return drawIndentations_;
}

void Qutepart::setDrawIndentations(bool draw) {
    drawIndentations_ = draw;
}

bool Qutepart::drawAnyWhitespace() const {
    return drawAnyWhitespace_;
}

void Qutepart::setDrawAnyWhitespace(bool draw) {
    drawAnyWhitespace_ = draw;
}

bool Qutepart::drawIncorrectIndentation() const {
    return drawIncorrectIndentation_;
}

void Qutepart::setDrawIncorrectIndentation(bool draw) {
    drawIncorrectIndentation_ = draw;
}

bool Qutepart::drawSolidEdge() const {
    return drawSolidEdge_;
}

void Qutepart::setDrawSolidEdge(bool draw) {
    drawSolidEdge_ = true;
    solidEdgeLine_->setVisible(draw);
    if (draw) {
        setSolidEdgeGeometry();
    }
}

int Qutepart::lineLengthEdge() const {
    return lineLengthEdge_;
}

void Qutepart::setLineLengthEdge(int edge) {
    lineLengthEdge_ = edge;
}

QColor Qutepart::lineLengthEdgeColor() const {
    return lineLengthEdgeColor_;
}

void Qutepart::setLineLengthEdgeColor(QColor color) {
    lineLengthEdgeColor_ = color;
}

bool Qutepart::bracketHighlightingEnabled() const {
    return bool(bracketHighlighter_);
}

void Qutepart::setBracketHighlightingEnabled(bool value) {
    if (value && ( ! bracketHighlighter_)) {
        bracketHighlighter_ = std::make_unique<BracketHighlighter>();
    } else if ( ( ! value) && bool(bracketHighlighter_)) {
        bracketHighlighter_.reset();
    }
    updateExtraSelections();
}

bool Qutepart::lineNumbersVisible() const {
    return bool(lineNumberArea_);
}

void Qutepart::setLineNumbersVisible(bool value) {
    if ( ( ! value) && bool(lineNumberArea_)) {
        lineNumberArea_.reset();
        updateViewport();
    } else if (value && (! bool(lineNumberArea_))) {
        lineNumberArea_ = std::make_unique<LineNumberArea>(this);
        connect(lineNumberArea_.get(), &LineNumberArea::widthChanged, this, &Qutepart::updateViewport);
    }
}

QAction* Qutepart::toggleBookmarkAction() const {
    return toggleBookmarkAction_;
}

QAction* Qutepart::prevBookmarkAction() const {
    return prevBookmarkAction_;
}

QAction* Qutepart::nextBookmarkAction() const {
    return nextBookmarkAction_;
}

void Qutepart::keyPressEvent(QKeyEvent *event) {
    QTextCursor cursor = textCursor();
    if (event->key() == Qt::Key_Backspace &&
        indenter_->shouldUnindentWithBackspace(cursor)) {
        // Unindent on backspace
        indenter_->onShortcutUnindentWithBackspace(cursor);
    } else if (event->matches(QKeySequence::InsertParagraphSeparator)) {
        // Enter pressed. Indent new empty line
        QPlainTextEdit::keyPressEvent(event);
        indenter_->indentBlock(cursor.block(), cursor.positionInBlock(), event->text()[0]);
    } else if (cursor.positionInBlock() == (cursor.block().length() - 1) &&
               indenter_->shouldAutoIndentOnEvent(event)) {
        // Indentation on special characters. Like closing bracket in XML
        QPlainTextEdit::keyPressEvent(event);
        indenter_->indentBlock(cursor.block(), cursor.positionInBlock(), event->text()[0]);
    } else {
        // make action shortcuts override keyboard events (non-default Qt behaviour)
        foreach(QAction* action, actions()) {
            QKeySequence seq = action->shortcut();
            if (seq.count() == 1 && seq[0] == (event->key() | int(event->modifiers()))) {
                action->trigger();
                return;
            }
        }

        QPlainTextEdit::keyPressEvent(event);
    }
}

void Qutepart::paintEvent(QPaintEvent *event) {
    QPlainTextEdit::paintEvent(event);
    drawIndentMarkersAndEdge(event->rect());
}

void Qutepart::initActions() {
    QAction* increaseIndentAction = createAction(
        "Increase indentation", QKeySequence(Qt::Key_Tab));
    connect(
        increaseIndentAction,
        &QAction::triggered,
        [=]{indenter_->onShortcutIndent(textCursor());}
    );

    toggleBookmarkAction_ = createAction(
        "Toggle bookmark", QKeySequence(Qt::CTRL | Qt::Key_B));
    connect(toggleBookmarkAction_, &QAction::triggered,
            this, &Qutepart::onShortcutToggleBookmark);

    prevBookmarkAction_ = createAction("Previous bookmark", QKeySequence(Qt::ALT | Qt::Key_Up));
    connect(prevBookmarkAction_, &QAction::triggered,
            this, &Qutepart::onShortcutPrevBookmark);

    nextBookmarkAction_ = createAction("Next bookmark", QKeySequence(Qt::ALT | Qt::Key_Down));
    connect(nextBookmarkAction_, &QAction::triggered,
            this, &Qutepart::onShortcutNextBookmark);
}

QAction* Qutepart::createAction(
    const QString& text, QKeySequence shortcut,
    const QString& iconFileName) {

    QAction* action = new QAction(text, this);
    // if iconFileName is not None:
    //     action.setIcon(getIcon(iconFileName))

    action->setShortcut(shortcut);
    action->setShortcutContext(Qt::WidgetShortcut);

    addAction(action);
    return action;
}


void Qutepart::drawIndentMarkersAndEdge(const QRect& paintEventRect) {
    QPainter painter(viewport());

    for(QTextBlock block = firstVisibleBlock(); block.isValid(); block = block.next()) {
        QRectF blockGeometry = blockBoundingGeometry(block).translated(contentOffset());
        if (blockGeometry.top() > paintEventRect.bottom()) {
            break;
        }

        if (block.isVisible() && blockGeometry.toRect().intersects(paintEventRect)) {
            // Draw indent markers, if good indentation is not drawn
            if (drawIndentations_ && (! drawAnyWhitespace_)) {
                QString text = block.text();
                QStringRef textRef(&text);
                int column = indenter_->width();
                while (textRef.startsWith(indenter_->indentText()) &&
                       textRef.length() > indenter_->width() &&
                       textRef.at(indenter_->width()).isSpace()) {
                    bool lineLengthMarkerHere = (column == lineLengthEdge_);
                    bool cursorHere = (block.blockNumber() == textCursor().blockNumber() &&
                         column == textCursor().columnNumber());
                    if ( ( ! lineLengthMarkerHere) &&
                         ( ! cursorHere)) { // looks ugly, if both drawn
                        // on some fonts line is drawn below the cursor, if offset is 1. Looks like Qt bug
                        drawIndentMarker(&painter, block, column);
                    }

                    textRef = textRef.mid(indenter_->width());
                    column += indenter_->width();
                }
            }

            // Draw edge, but not over a cursor
            if ( ! drawSolidEdge_) {
                int edgePos = effectiveEdgePos(block.text());
                if (edgePos != -1 && edgePos != textCursor().columnNumber()) {
                    drawEdgeLine(&painter, block, edgePos);
                }
            }

            if (drawAnyWhitespace_ || drawIncorrectIndentation_) {
                QString text = block.text();
                QVector<bool> visibleFlags(text.length());
                chooseVisibleWhitespace(text, &visibleFlags);
                for(int column = 0; column < visibleFlags.length(); column++) {
                    bool draw = visibleFlags[column];
                    if (draw) {
                        drawWhiteSpace(&painter, block, column, text[column]);
                    }
                }
            }
        }
    }
}

void Qutepart::drawWhiteSpace(QPainter* painter, QTextBlock block, int column, QChar ch) {
    QRect leftCursorRect = cursorRect(block, column, 0);
    QRect rightCursorRect = cursorRect(block, column + 1, 0);
    if (leftCursorRect.top() == rightCursorRect.top()) {  // if on the same visual line
        int middleHeight = (leftCursorRect.top() + leftCursorRect.bottom()) / 2;
        if (ch == ' ') {
            painter->setPen(Qt::transparent);
            painter->setBrush(QBrush(Qt::gray));
            int xPos = (leftCursorRect.x() + rightCursorRect.x()) / 2;
            painter->drawRect(QRect(xPos, middleHeight, 2, 2));
        } else {
            painter->setPen(QColor(Qt::gray).lighter(120));
            painter->drawLine(leftCursorRect.x() + 3, middleHeight,
                             rightCursorRect.x() - 3, middleHeight);
        }
    }
}

int Qutepart::effectiveEdgePos(const QString& text) {
    /* Position of edge in a block.
     * Defined by lineLengthEdge, but visible width of \t is more than 1,
     * therefore effective position depends on count and position of \t symbols
     * Return -1 if line is too short to have edge
     */
    if (lineLengthEdge_ <= 0) {
        return -1;
    }

    int tabExtraWidth = indenter_->width() - 1;
    int fullWidth = text.length() + (text.count('\t') * tabExtraWidth);
    int indentWidth = indenter_->width();

    if (fullWidth <= lineLengthEdge_) {
        return -1;
    }

    int currentWidth = 0;
    for(int pos = 0; pos < text.length(); pos++) {
        if (text[pos] == '\t') {
            // Qt indents up to indentation level, so visible \t width depends on position
            currentWidth += (indentWidth - (currentWidth % indentWidth));
        } else {
            currentWidth += 1;
        }
        if (currentWidth > lineLengthEdge_) {
            return pos;
        }
    }

    // line too narrow, probably visible \t width is small
    return -1;
}

void Qutepart::chooseVisibleWhitespace(const QString& text, QVector<bool>* result) {
    int lastNonSpaceColumn = text.length() - 1;
    while (text[lastNonSpaceColumn].isSpace()) {
        lastNonSpaceColumn--;
    }

    // Draw not trailing whitespace
    if (drawAnyWhitespace_) {
        // Any
        for (int column = 0; column < lastNonSpaceColumn; column++) {
            QChar ch = text[column];
            if (ch.isSpace() &&
                (ch == '\t' || column == 0 || text[column - 1].isSpace() ||
                 ((column + 1) < lastNonSpaceColumn &&
                  text[column + 1].isSpace()))) {
                result->replace(column, true);
            }
        }
    } else if (drawIncorrectIndentation_) {
        // Only incorrect
        if (indenter_->useTabs()) {
            // Find big space groups
            QString bigSpaceGroup = QString().fill(' ', indenter_->width());
            for(int column = text.indexOf(bigSpaceGroup);
                column != -1 && column < lastNonSpaceColumn;
                column = text.indexOf(bigSpaceGroup, column))
            {
                int index = -1;
                for (index = column; index < column + indenter_->width(); index++) {
                    result->replace(index, true);
                }
                for (; index < lastNonSpaceColumn && text[index] == ' '; index++) {
                        result->replace(index, true);
                }
                column = index;
            }
        } else {
            // Find tabs:
            for(int column = text.indexOf('\t');
                column != -1 && column < lastNonSpaceColumn;
                column = text.indexOf('\t', column))
            {
                result->replace(column, true);
                column += 1;
            }
        }
    }

    // Draw trailing whitespace
    if (drawIncorrectIndentation_ || drawAnyWhitespace_) {
        for (int column = lastNonSpaceColumn + 1; column < text.length(); column++) {
            result->replace(column, true);
        }
    }
}

void Qutepart::setSolidEdgeGeometry() {
    // TODO call when viewport resized
    // Sets the solid edge line geometry if needed
    if (lineLengthEdge_ > 0) {
        QRect cr = contentsRect();

        // contents margin usually gives 1
        // cursor rectangle left edge for the very first character usually
        // gives 4
        int x = fontMetrics().width(QString().fill('9', lineLengthEdge_)) +
            /* self._totalMarginWidth + */
            /* self.contentsMargins().left() + */
            cursorRect(firstVisibleBlock(), 0, 0).left();
        solidEdgeLine_->setGeometry(QRect(x, cr.top(), 1, cr.bottom()));
    }
}

void Qutepart::updateViewport() {
    // Recalculates geometry for all the margins and the editor viewport
    QRect cr = contentsRect();
    int currentX = cr.left();
    int top = cr.top();
    int height = cr.height();

    int totalMarginWidth = 0;

    if(lineNumberArea_) {
        int width = lineNumberArea_->widthHint();
        lineNumberArea_->setGeometry(QRect(currentX, top, width, height));
        currentX += width;
        totalMarginWidth += width;
    }

    {
        int width = markArea_->widthHint();
        markArea_->setGeometry(QRect(currentX, top, width, height));
        currentX += width;
        totalMarginWidth += width;
    }

    if (totalMarginWidth_ != totalMarginWidth) {
        totalMarginWidth_ = totalMarginWidth;
        updateViewportMargins();
    } else {
        setSolidEdgeGeometry();
    }
}

void Qutepart::updateViewportMargins() {
    // Sets the viewport margins and the solid edge geometry
    setViewportMargins(totalMarginWidth_, 0, 0, 0);
    setSolidEdgeGeometry();
}

void Qutepart::resizeEvent(QResizeEvent* event) {
    QPlainTextEdit::resizeEvent(event);
    updateViewport();
}

void Qutepart::updateTabStopWidth() {
    // Update tabstop width after font or indentation changed
    int width = fontMetrics().width(QString().fill(' ', indenter_->width()));
    setTabStopWidth(width);
}

void Qutepart::drawIndentMarker(QPainter* painter, QTextBlock block, int column) {
    painter->setPen(QColor(Qt::blue).lighter());
    QRect rect = cursorRect(block, column, 0);
    painter->drawLine(rect.topLeft(), rect.bottomLeft());
}

void Qutepart::drawEdgeLine(QPainter* painter, QTextBlock block, int edgePos) {
    painter->setPen(QPen(QBrush(lineLengthEdgeColor_), 0));
    QRect rect = cursorRect(block, edgePos, 0);
    painter->drawLine(rect.topLeft(), rect.bottomLeft());
}

QRect Qutepart::cursorRect(QTextBlock block, int column, int offset) const {
    QTextCursor cursor(block);
    setPositionInBlock(&cursor, column);
    return QPlainTextEdit::cursorRect(cursor).translated(offset, 0);
}

void Qutepart::gotoBlock(const QTextBlock& block) {
    QTextCursor cursor(block);
    setTextCursor(cursor);
}

void Qutepart::updateExtraSelections() {
    QTextCursor cursor = textCursor();
    QList<QTextEdit::ExtraSelection> bracketSelections;

    if (bracketHighlighter_) {
        bracketSelections = bracketHighlighter_->extraSelections(
                TextPosition(textCursor().block(),
                             cursor.positionInBlock()));
    }

    setExtraSelections(bracketSelections);
}

void Qutepart::onShortcutToggleBookmark() {
    QTextBlock block = textCursor().block();
    bool value = ! isBookmarked(block);
    setBookmarked(block, value);
    markArea_->update();
}

void Qutepart::onShortcutPrevBookmark() {
    QTextBlock currentBlock = textCursor().block();
    QTextBlock block = currentBlock.previous();

    while(block.isValid()) {
        if (isBookmarked(block)) {
            gotoBlock(block);
            return;
        }
        block = block.previous();
    }
    block = document()->lastBlock();
    while (block != currentBlock) {
        if (isBookmarked(block)) {
            gotoBlock(block);
            return;
        }
        block = block.previous();
    }
}

void Qutepart::onShortcutNextBookmark() {
    QTextBlock currentBlock = textCursor().block();
    QTextBlock block = currentBlock.next();

    while(block.isValid()) {
        if (isBookmarked(block)) {
            gotoBlock(block);
            return;
        }
        block = block.next();
    }

    block = document()->firstBlock();
    while (block != currentBlock) {
        if (isBookmarked(block)) {
            gotoBlock(block);
            return;
        }
        block = block.next();
    }
}

}  // namespace Qutepart
