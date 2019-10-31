#include "indenter.h"

namespace Qutepart {

namespace {

int firstNonSpaceColumn(const QString& line) {
    for(int i = 0; i < line.size(); i++) {
        if ( ! line[i].isSpace()) {
            return i;
        }
    }
    return line.size();
}

QString lineIndent(const QString& line) {
    return line.left(firstNonSpaceColumn(line));
}

QString prevBlockIndent(QTextBlock block) {
    QTextBlock prevBlock = block.previous();

    if ( ! block.isValid()) {
            return QString::null;
    }

    return lineIndent(prevBlock.text());
}


class NoneIndentAlg: public IndentAlg {
public:
    const QString& triggerCharacters() const override {
        return TRIGGER_CHARACTERS;
    }

    QString computeSmartIndent(QTextBlock block, QChar typedKey) const override {
        return "";  // TODO
    }

private:
    static const QString TRIGGER_CHARACTERS;
};

const QString NoneIndentAlg::TRIGGER_CHARACTERS = QString("");

}  // namespace

Indenter::Indenter():
    alg(new NoneIndentAlg())
{
}

Indenter::~Indenter() {
    delete alg;
}

bool Indenter::shouldAutoIndentOnEvent(QKeyEvent* event) const {
    return ( ! event->text().isEmpty() &&
            alg->triggerCharacters().contains(event->text()));
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
    if (typedKey == '\n' &&
        prevBlockText.trimmed().isEmpty()) {  // continue indentation, if no text
        return prevBlockIndent(block);
    } else {  // be smart
        return alg->computeSmartIndent(block, typedKey);
    }
}

}  // namespace Qutepart
