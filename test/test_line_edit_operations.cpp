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

    void MoveDownOneLineEolAtEnd() {
        Qutepart::Qutepart qpart(nullptr, "one\ntwo\nthree\nfour\n");

        QTest::keyClick(&qpart, Qt::Key_Down, Qt::AltModifier);
        QCOMPARE(qpart.toPlainText(), QString("two\none\nthree\nfour\n"));

        QTest::keyClick(&qpart, Qt::Key_Down, Qt::AltModifier);
        QCOMPARE(qpart.toPlainText(), QString("two\nthree\none\nfour\n"));

        QTest::keyClick(&qpart, Qt::Key_Down, Qt::AltModifier);
        QCOMPARE(qpart.toPlainText(), QString("two\nthree\nfour\none\n"));

        QTest::keyClick(&qpart, Qt::Key_Down, Qt::AltModifier);
        QCOMPARE(qpart.toPlainText(), QString("two\nthree\nfour\n\none"));

        QTest::keyClick(&qpart, Qt::Key_Down, Qt::AltModifier);
        QCOMPARE(qpart.toPlainText(), QString("two\nthree\nfour\n\none"));

        qpart.undo();
        QCOMPARE(qpart.toPlainText(), QString("two\nthree\nfour\none\n"));
        qpart.undo();
        QCOMPARE(qpart.toPlainText(), QString("two\nthree\none\nfour\n"));
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

    void MoveDownTwoLines() {
        Qutepart::Qutepart qpart(nullptr, "one\ntwo\nthree\nfour");

        QTextCursor cursor = qpart.textCursor();
        cursor.setPosition(1);
        cursor.setPosition(6, QTextCursor::KeepAnchor);
        qpart.setTextCursor(cursor);
        QCOMPARE(qpart.textCursor().selectedText(), QString("ne\u2029tw"));

        QTest::keyClick(&qpart, Qt::Key_Down, Qt::AltModifier);
        QCOMPARE(qpart.toPlainText(), QString("three\none\ntwo\nfour"));

        QTest::keyClick(&qpart, Qt::Key_Down, Qt::AltModifier);
        QCOMPARE(qpart.toPlainText(), QString("three\nfour\none\ntwo"));

        QTest::keyClick(&qpart, Qt::Key_Down, Qt::AltModifier);
        QCOMPARE(qpart.toPlainText(), QString("three\nfour\none\ntwo"));

        QCOMPARE(qpart.textCursor().selectedText(), QString("ne\u2029tw"));

        qpart.undo();
        QCOMPARE(qpart.toPlainText(), QString("three\none\ntwo\nfour"));
        qpart.undo();
        QCOMPARE(qpart.toPlainText(), QString("one\ntwo\nthree\nfour"));
    }

    void MoveUpTwoLines() {
        Qutepart::Qutepart qpart(nullptr, "one\ntwo\nthree\nfour");

        QTextCursor cursor = qpart.textCursor();
        cursor.setPosition(10);
        cursor.setPosition(16, QTextCursor::KeepAnchor);
        qpart.setTextCursor(cursor);
        QCOMPARE(qpart.textCursor().selectedText(), QString("ree\u2029fo"));

        QTest::keyClick(&qpart, Qt::Key_Up, Qt::AltModifier);
        QCOMPARE(qpart.toPlainText(), QString("one\nthree\nfour\ntwo"));

        QTest::keyClick(&qpart, Qt::Key_Up, Qt::AltModifier);
        QCOMPARE(qpart.toPlainText(), QString("three\nfour\none\ntwo"));

        QTest::keyClick(&qpart, Qt::Key_Up, Qt::AltModifier);
        QCOMPARE(qpart.toPlainText(), QString("three\nfour\none\ntwo"));

        QCOMPARE(qpart.textCursor().selectedText(), QString("ree\u2029fo"));

        qpart.undo();
        QCOMPARE(qpart.toPlainText(), QString("one\nthree\nfour\ntwo"));
        qpart.undo();
        QCOMPARE(qpart.toPlainText(), QString("one\ntwo\nthree\nfour"));
    }

    void DuplicateLine() {
        Qutepart::Qutepart qpart(nullptr, "one\ntwo\n   three\nfour");

        QTextCursor cursor = qpart.textCursor();

        qpart.goToLine(1);
        QTest::keyClick(&qpart, Qt::Key_D, Qt::AltModifier);
        QCOMPARE(qpart.toPlainText(), QString("one\ntwo\ntwo\n   three\nfour"));
        QCOMPARE(qpart.textCursor().blockNumber(), 2);
        QCOMPARE(qpart.textCursor().positionInBlock(), 0);
    }

    void DuplicateIndentedLine() {
        Qutepart::Qutepart qpart(nullptr, "one\ntwo\n   three\nfour");

        QTextCursor cursor = qpart.textCursor();

        qpart.goToLine(2);
        QTest::keyClick(&qpart, Qt::Key_D, Qt::AltModifier);
        QCOMPARE(qpart.toPlainText(), QString("one\ntwo\n   three\n   three\nfour"));
        QCOMPARE(qpart.textCursor().blockNumber(), 3);
        QCOMPARE(qpart.textCursor().positionInBlock(), 3);
    }
};

QTEST_MAIN(Test)
#include "test_line_edit_operations.moc"
