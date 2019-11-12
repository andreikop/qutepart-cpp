#include <QtTest/QtTest>
#include <QTextCursor>

#include "qutepart.h"


class BaseTest: public QObject {
protected:
    Qutepart::Qutepart qpart;

    void init() {  // called by the framework
        qpart.setPlainText("");
    }

    void setCursorPosition(int line, int col) {
        QTextCursor cursor(qpart.document()->findBlockByNumber(line));
        cursor.setPosition(cursor.block().position() + col);
        qpart.setTextCursor(cursor);
    }

    void enter() {
        QTest::keyClick(&qpart, Qt::Key_Enter);
    }

    void type(const QString& text) {
        QTest::keyClicks(&qpart, text);
    }

    void verifyExpected(const QString& expected) {
        QCOMPARE(qpart.toPlainText(), expected);
    }
};


class Test: public BaseTest
{
    Q_OBJECT
private slots:

    void test_normal2() {
        QString origin(
            "    bla bla\n");

        QString expected(
            "    bla bla\n"
            "ok");

        qpart.setPlainText(origin);

        setCursorPosition(0,11);
        enter();
        type("ok");
        verifyExpected(expected);
    }
};


QTEST_MAIN(Test)
#include "indenter.moc"
