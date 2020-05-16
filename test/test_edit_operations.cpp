#include <QDebug>
#include <QObject>
#include <QTest>

#include "qutepart.h"


class Test: public QObject {
    Q_OBJECT

private slots:
    void SmartHome() {
        Qutepart::Qutepart qpart(nullptr, "one\n    two");

        QTest::keyClick(&qpart, Qt::Key_Home);
        QCOMPARE(qpart.textCursor().positionInBlock(), 0);

        QTest::keyClick(&qpart, Qt::Key_Home);
        QCOMPARE(qpart.textCursor().positionInBlock(), 0);

        qpart.goTo(1, 2);
        QTest::keyClick(&qpart, Qt::Key_Home);
        QCOMPARE(qpart.textCursor().positionInBlock(), 4);

        QTest::keyClick(&qpart, Qt::Key_Home);
        QCOMPARE(qpart.textCursor().positionInBlock(), 0);

        QTest::keyClick(&qpart, Qt::Key_Home);
        QCOMPARE(qpart.textCursor().positionInBlock(), 4);
    }

    void JoinLines() {
        Qutepart::Qutepart qpart(nullptr, "one\ntwo\n    three");

        QTest::keyClick(&qpart, Qt::Key_J, Qt::ControlModifier);
        QCOMPARE(qpart.toPlainText(), QString("one two\n    three"));

        QTest::keyClick(&qpart, Qt::Key_J, Qt::ControlModifier);
        QCOMPARE(qpart.toPlainText(), QString("one two three"));

        QTest::keyClick(&qpart, Qt::Key_J, Qt::ControlModifier);
        QCOMPARE(qpart.toPlainText(), QString("one two three"));
    }
};

QTEST_MAIN(Test)
#include "test_edit_operations.moc"
