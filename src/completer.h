#pragma once

#include <memory>
#include <QObject>
#include <QSet>
#include <QTimer>

namespace Qutepart {

class Qutepart;

class CompletionList;
class CompletionModel;

class Completer: public QObject {
    Q_OBJECT

public:
    Completer(Qutepart* qpart);
    ~Completer();

private slots:
    void onTextChanged();
    void onModificationChanged(bool modified);
    void onCompletionListItemSelected(int index);
    void onCompletionListTabPressed();

private:
    void setKeywords(const QSet<QString>& keywords);
    void setCustomCompletions(const QSet<QString>& wordSet);
    bool isVisible() const;
    void updateWordSet();
    void invokeCompletion();
    bool shouldShowModel(CompletionModel* model, bool forceShow);
    CompletionList* createWidget(CompletionModel* model);
    bool invokeCompletionIfAvailable(bool requestedByUser);
    void closeCompletion();
    QString getWordBeforeCursor() const;
    QString getWordAfterCursor() const;

    Qutepart* qpart_;
    std::unique_ptr<CompletionList> widget_;
    bool completionOpenedManually_;
    QSet<QString> keywords_;
    QSet<QString> customCompletions_;
    QSet<QString> wordSet_;
    QTimer updateWordSetTimer_;
};

};  // namespace Qutepart
