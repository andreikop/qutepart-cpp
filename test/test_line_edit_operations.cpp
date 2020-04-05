#include <QDebug>
#include <QObject>
#include <QTest>

#include "qutepart.h"


class Test: public QObject {
    Q_OBJECT

private slots:
    void MoveDownOneLine() {
        Qutepart::Qutepart qpart(nullptr, "one\ntwo\nthree\nfour");

        QTest::keyClick(&qpart, Qt::Key_Down, Qt::AltModifier);
        QCOMPARE(qpart.toPlainText(), QString("two\none\nthree\nfour"));

        QTest::keyClick(&qpart, Qt::Key_Down, Qt::AltModifier);
        QCOMPARE(qpart.toPlainText(), QString("two\nthree\none\nfour"));

        QTest::keyClick(&qpart, Qt::Key_Down, Qt::AltModifier);
        QCOMPARE(qpart.toPlainText(), QString("two\nthree\nfour\none"));

        QTest::keyClick(&qpart, Qt::Key_Down, Qt::AltModifier);
        QCOMPARE(qpart.toPlainText(), QString("two\nthree\nfour\none"));

        qpart.undo();
        QCOMPARE(qpart.toPlainText(), QString("two\nthree\none\nfour"));
        qpart.undo();
        QCOMPARE(qpart.toPlainText(), QString("two\none\nthree\nfour"));
    }

    void MoveUpOneLine() {
        Qutepart::Qutepart qpart(nullptr, "one\ntwo\nthree\nfour");

        qpart.goToLine(3);

        QTest::keyClick(&qpart, Qt::Key_Right);
        QCOMPARE(qpart.textCursor().positionInBlock(), 1);

        QTest::keyClick(&qpart, Qt::Key_Up, Qt::AltModifier);
        QCOMPARE(qpart.toPlainText(), QString("one\ntwo\nfour\nthree"));

        QTest::keyClick(&qpart, Qt::Key_Up, Qt::AltModifier);
        QCOMPARE(qpart.toPlainText(), QString("one\nfour\ntwo\nthree"));

        QTest::keyClick(&qpart, Qt::Key_Up, Qt::AltModifier);
        QCOMPARE(qpart.toPlainText(), QString("four\none\ntwo\nthree"));

        QTest::keyClick(&qpart, Qt::Key_Up, Qt::AltModifier);
        QCOMPARE(qpart.toPlainText(), QString("four\none\ntwo\nthree"));

        QCOMPARE(qpart.textCursor().positionInBlock(), 1);
    }
};

QTEST_MAIN(Test)
#include "test_line_edit_operations.moc"
