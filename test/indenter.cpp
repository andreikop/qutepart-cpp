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


typedef std::pair<int,int> CursorPos;

class Test: public BaseTest
{
    Q_OBJECT
private slots:

    void test_data() {
        QTest::addColumn<QString>("origin");
        QTest::addColumn<CursorPos>("cursorPos");
        QTest::addColumn<QString>("input");
        QTest::addColumn<QString>("expected");

        QTest::newRow("normal 2")
            << "    bla bla"
            << std::make_pair(0, 11)
            << "\nok"
            << "    bla bla\n"
               "    ok";
    }

    void test() {
        QFETCH(QString, origin);
        QFETCH(CursorPos, cursorPos);
        QFETCH(QString, input);
        QFETCH(QString, expected);

        qpart.setPlainText(origin);

        setCursorPosition(cursorPos.first, cursorPos.second);

        for (auto ch = input.begin(); ch != input.end(); ++ch) {
            if (*ch == '\n') {
                enter();
            } else {
                type(*ch);
            }
        }
        verifyExpected(expected);
    }
};


QTEST_MAIN(Test)
#include "indenter.moc"
