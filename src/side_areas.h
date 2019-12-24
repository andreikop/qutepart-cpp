#include <QWidget>
#include <QPlainTextEdit>

namespace Qutepart {

class Qutepart;

class LineNumberArea: public QWidget {
    Q_OBJECT

public:
    LineNumberArea(Qutepart* textEdit);

    int widthHint() const;

signals:
    void widthChanged();

private slots:
    void updateWidth();

private:
    void paintEvent(QPaintEvent* event);
    void changeEvent(QEvent* event) override;

    Qutepart* textEdit_;
    int desiredWidth_;
};

}  // namespace Qutepart
