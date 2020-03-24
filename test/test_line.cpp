#include <QDebug>
#include <QObject>
#include <QTest>

#include "qutepart.h"


class Test: public QObject {
    Q_OBJECT

private slots:
    void doTest(const QString& text, int pos, int count, const QString& expected) {
        Qutepart::Qutepart qpart(nullptr, text);

        Qutepart::Line line(qpart.document()->firstBlock());

        line.remove(pos, count);

        QCOMPARE(line.text(), expected);
    }

    void RemoveAll() {
        doTest("asdfghjkl", 0, 9, "");
    }

    void RemoveFirst() {
        doTest("asdfghjkl", 0, 1, "sdfghjkl");
    }

    void RemoveLast() {
        doTest("asdfghjkl", 8, 1, "asdfghjk");
    }

    void RemoveMid() {
        doTest("asdfghjkl", 2, 5, "askl");
    }

    void RemoveZero() {
        doTest("asdfghjkl", 9, 0, "asdfghjkl");
        doTest("asdfghjkl", 0, 0, "asdfghjkl");
    }
};

QTEST_MAIN(Test)
#include "test_line.moc"
