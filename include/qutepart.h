#pragma once

#include <memory>

#include <QColor>
#include <QPlainTextEdit>
#include <QSharedPointer>
#include <QSyntaxHighlighter>


namespace Qutepart {

enum IndentAlg {
    INDENT_ALG_NONE = 0,
    INDENT_ALG_NORMAL,
    INDENT_ALG_CSTYLE,
    INDENT_ALG_LISP,
    INDENT_ALG_SCHEME,
    INDENT_ALG_XML,
    INDENT_ALG_PYTHON,
    INDENT_ALG_RUBY,
};


struct LangInfo {
    QString id;  // Internal unique language ID
    QStringList names;   // user readable language names
    IndentAlg indentAlg;   // indenter algorithm for the language

    LangInfo() = default;

    inline LangInfo(const QString& id, const QStringList& names, IndentAlg indentAlg):
        id(id),
        names(names),
        indentAlg(indentAlg)
    {};

    inline bool isValid() const {
        return ! id.isEmpty();
    }
};

/* Choose language by available parameters
 * First parameters have higher priority
 * Returns QString::null if failed
 */
LangInfo chooseLanguage(
    const QString& mimeType=QString::null,
    const QString& languageName=QString::null,
    const QString& sourceFilePath=QString::null,
    const QString& firstLine=QString::null);

class Indenter;
class BracketHighlighter;
class LineNumberArea;
class MarkArea;

class Qutepart: public QPlainTextEdit {
    Q_OBJECT

public:
    Qutepart(QWidget *parent = nullptr, const QString &text = QString::null);

    // Not copyable or movable
    Qutepart(const Qutepart&) = delete;
    Qutepart& operator=(const Qutepart&) = delete;
    Qutepart(Qutepart&&) = delete;
    Qutepart& operator=(Qutepart&&) = delete;

    virtual ~Qutepart();

    // Set highlighter. Use chooseLanguage() to get the id
    void setHighlighter(const QString& languageId);

    // Set indenter algorithm. Use chooseLanguage() to choose algorithm
    void setIndentAlgorithm(IndentAlg indentAlg);

    // Indent current line using current smart indentation algorithm
    void autoIndentCurrentLine();

    // Editor configuration
    bool indentUseTabs() const;
    void setIndentUseTabs(bool);

    int indentWidth() const;
    void setIndentWidth(int);

    // Editor apperance configuration
    bool drawIndentations() const;
    void setDrawIndentations(bool);

    bool drawAnyWhitespace() const;
    void setDrawAnyWhitespace(bool);

    bool drawIncorrectIndentation() const;
    void setDrawIncorrectIndentation(bool);

    bool drawSolidEdge() const;
    void setDrawSolidEdge(bool);

    int lineLengthEdge() const;
    void setLineLengthEdge(int);

    QColor lineLengthEdgeColor() const;
    void setLineLengthEdgeColor(QColor);

    QColor currentLineColor() const;
    void setCurrentLineColor(QColor);

    bool bracketHighlightingEnabled() const;
    void setBracketHighlightingEnabled(bool value);

    bool lineNumbersVisible() const;
    void setLineNumbersVisible(bool value);

    void setCompletionEnabled(bool);
    bool completionEnabled() const;

    void setCompletionThreshold(int);
    int completionThreshold() const;

    QAction* increaseIndentAction() const;
    QAction* decreaseIndentAction() const;

    QAction* toggleBookmarkAction() const;
    QAction* prevBookmarkAction() const;
    QAction* nextBookmarkAction() const;

    QRect cursorRect(QTextBlock block, int column, int offset) const;
protected:
    void keyPressEvent(QKeyEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void changeEvent(QEvent *event) override;

private:
    void initActions();
    QAction* createAction(const QString& text, QKeySequence shortcut,
                          const QString& iconFileName=QString::null);

    // whitespace and edge drawing
    void drawIndentMarkersAndEdge(const QRect& rect);
    void drawIndentMarker(QPainter* painter, QTextBlock block, int column);
    void drawEdgeLine(QPainter* painter, QTextBlock block, int edgePos);
    void drawWhiteSpace(QPainter* painter, QTextBlock block, int column, QChar ch);
    int effectiveEdgePos(const QString& text);
    void chooseVisibleWhitespace(const QString& text, QVector<bool>* result);

    QTextEdit::ExtraSelection currentLineExtraSelection() const;

    void setSolidEdgeGeometry();
    void resizeEvent(QResizeEvent* event) override;

    void updateTabStopWidth();

    void gotoBlock(const QTextBlock& block);

    void indentBlock(const QTextBlock& block, bool withSpace) const;
    void unIndentBlock(const QTextBlock& block, bool withSpace) const;
    void changeSelectedBlocksIndent(bool increase, bool withSpace);

private slots:
    void updateViewport();
    void updateExtraSelections();

    void onShortcutToggleBookmark();
    void onShortcutPrevBookmark();
    void onShortcutNextBookmark();

private:
    QSharedPointer<QSyntaxHighlighter> highlighter_;
    std::unique_ptr<Indenter> indenter_;
    std::unique_ptr<BracketHighlighter> bracketHighlighter_;
    std::unique_ptr<LineNumberArea> lineNumberArea_;
    std::unique_ptr<MarkArea> markArea_;

    bool drawIndentations_;
    bool drawAnyWhitespace_;
    bool drawIncorrectIndentation_;
    bool drawSolidEdge_;
    int lineLengthEdge_;
    QColor lineLengthEdgeColor_;
    QColor currentLineColor_;

    bool completionEnabled_;
    bool completionThreshold_;

    QWidget* solidEdgeLine_;
    int totalMarginWidth_;

    QAction* increaseIndentAction_;
    QAction* decreaseIndentAction_;

    QAction* toggleBookmarkAction_;
    QAction* prevBookmarkAction_;
    QAction* nextBookmarkAction_;

    friend class LineNumberArea;
    friend class MarkArea;
};

/*
A helper class which allows to group edit operations on Qutepart using RAII approach
Operations are undo-redoble as single change.
Example
    {
        AtomicOperation op(qutepart);
        qutepart.editText();
        qutepart.editMoreText();
    }
 */
class AtomicEditOperation {
public:
    AtomicEditOperation(Qutepart* qutepart);
    ~AtomicEditOperation();

private:
    Qutepart* qutepart_;
};

}; // namespace Qutepart
