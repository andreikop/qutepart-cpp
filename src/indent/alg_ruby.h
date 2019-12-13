#pragma once

#include <QString>
#include <QTextBlock>

#include "text_block_utils.h"

#include "indenter.h"


namespace Qutepart {

class RubyStatement {
public:
    RubyStatement(QTextBlock startBlock, QTextBlock endBlock);

    // Convert to string for debugging
    QString toString() const;

#if 0
    TextPosition offsetToCursor(int offset) const;
#endif

    // Return document.isCode at the given offset in a statement
    bool isCode(int offset) const;

    // Return document.isComment at the given offset in a statement
    bool isComment(int offset) const;

    // Return the indent at the beginning of the statement
    QString indent() const;
    // Return the content of the statement from the document
    QString content() const;

    QTextBlock startBlock;
    QTextBlock endBlock;
};


class IndentAlgRuby: public IndentAlgImpl {
public:
    const QString& triggerCharacters() const override;
    QString computeSmartIndent(QTextBlock block) const override;

private:
    bool isCommentBlock(QTextBlock block) const;
    bool isComment(QTextBlock block, int column) const;
    QTextBlock prevNonCommentBlock(QTextBlock block) const;
    bool isLastCodeColumn(QTextBlock block, int column) const;
    bool isStmtContinuing(QTextBlock block) const;
    QTextBlock findStmtStart(QTextBlock block) const;
    bool isValidTrigger(QTextBlock block) const;

    RubyStatement findPrevStmt(QTextBlock block) const;
    bool isBlockStart(const RubyStatement& stmt) const;
    bool isBlockEnd(const RubyStatement& stmt) const;

    RubyStatement findBlockStart(QTextBlock block) const;
};

};  // namespace Qutepart
