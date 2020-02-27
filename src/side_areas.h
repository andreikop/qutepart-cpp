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
    void paintEvent(QPaintEvent* event) override;
    void changeEvent(QEvent* event) override;

    Qutepart* textEdit_;
    int desiredWidth_;
};


class MarkArea: public QWidget {
public:
    MarkArea(Qutepart* qpart);
    int widthHint() const;

private:
    QPixmap loadIcon(const QString& name) const;

    void paintEvent(QPaintEvent* event) override;

#if 0  // TODO
    void mouseMoveEvent(QMouseEvent* event);
#endif

    QPixmap bookmarkPixmap_;
    Qutepart* qpart_;
};

}  // namespace Qutepart
