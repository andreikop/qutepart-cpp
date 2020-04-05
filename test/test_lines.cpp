#include <QDebug>
#include <QObject>
#include <QTest>

#include "qutepart.h"


class Test: public QObject {
    Q_OBJECT

private slots:
    void Append() {
        QString text = "asdf\nfd";
        Qutepart::Qutepart qpart(nullptr, text);
        qpart.lines().append("xyz");
        QCOMPARE(qpart.toPlainText(), QString("asdf\nfd\nxyz"));

        qpart.undo();
        QCOMPARE(qpart.toPlainText(), text);
    }

    void Pop_data() {
        QTest::addColumn<QString>("text");
        QTest::addColumn<int>("lineNumber");
        QTest::addColumn<QString>("removed");
        QTest::addColumn<QString>("expected");

        QTest::newRow("first")
                << "one\ntwo\nthree"
                << 0
                << "one"
                << "two\nthree";

        QTest::newRow("middle")
                << "one\ntwo\nthree"
                << 1
                << "two"
                << "one\nthree";

        QTest::newRow("last")
                << "one\ntwo\nthree"
                << 2
                << "three"
                << "one\ntwo";
    }

    void Pop() {
        QFETCH(QString, text);
        QFETCH(int, lineNumber);
        QFETCH(QString, removed);
        QFETCH(QString, expected);

        Qutepart::Qutepart qpart(nullptr, text);
        QString actualRemoved = qpart.lines().popAt(lineNumber);
        QCOMPARE(actualRemoved, removed);
        QCOMPARE(qpart.toPlainText(), expected);

        qpart.undo();
        QCOMPARE(qpart.toPlainText(), text);
    }
};

QTEST_MAIN(Test)
#include "test_lines.moc"
