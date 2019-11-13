#include <QtTest/QtTest>
#include <QTextCursor>

#include "qutepart.h"


typedef std::pair<int,int> CursorPos;


class BaseTest: public QObject {
    Q_OBJECT

protected:
    Qutepart::Qutepart qpart;

    void setCursorPosition(int line, int col) {
        QTextCursor cursor(qpart.document()->findBlockByNumber(line));
        cursor.setPosition(cursor.block().position() + col);
        qpart.setTextCursor(cursor);
    }

    void enter() {
        QTest::keyClick(&qpart, Qt::Key_Enter);
    }

    void tab() {
        QTest::keyClick(&qpart, Qt::Key_Tab);
    }

    void type(const QString& text) {
        QTest::keyClicks(&qpart, text);
    }

    void verifyExpected(const QString& expected) {
        QCOMPARE(qpart.toPlainText(), expected);
    }

    void runDataDrivenTest() {
        QFETCH(QString, origin);
        QFETCH(CursorPos, cursorPos);
        QFETCH(QString, input);
        QFETCH(QString, expected);

        qpart.setPlainText(origin);

        setCursorPosition(cursorPos.first, cursorPos.second);

        for (auto ch = input.begin(); ch != input.end(); ++ch) {
            if (*ch == '\n') {
                enter();
            } else if (*ch == '\t') {
                tab();
            } else {
                type(*ch);
            }
        }
        verifyExpected(expected);
    }

private slots:
    void initTestCase() {
         Q_INIT_RESOURCE(qutepart_syntax_files);
    }

    virtual void init() {
        qpart.setPlainText("");
    }

};


class Test: public BaseTest
{
    Q_OBJECT

private:
    void addColumns() {
        QTest::addColumn<QString>("origin");
        QTest::addColumn<CursorPos>("cursorPos");
        QTest::addColumn<QString>("input");
        QTest::addColumn<QString>("expected");
    }
private slots:

    void normal_data() {
        addColumns();
        QTest::newRow("normal 2")
            << "    bla bla"
            << std::make_pair(0, 11)
            << "\nok"
            << "    bla bla\n"
               "    ok";

#if 0 // Now empty lines are not removed
        QTest::newRow("empty line 3")
            << "totally empty line\n"
               "\n"
               "\n"
            << std::make_pair(1, 0)
            << "\n\nok"
            << "    totally empty line\n"
               "\n"
               "\n"
               "    ok\n";
#endif

        QTest::newRow("empty line 1")
            <<  "      totally empty line\n"
                "\n"
            << std::make_pair(1, 0)
            <<  "\n\tok"
            <<  "      totally empty line\n"
                "\n"
                "      ok\n";

        QTest::newRow("normal 3")
            <<  "    bla bla\n"
                "    blu blu\n"
            << std::make_pair(1, 11)
            << "\nok"
            <<  "    bla bla\n"
                "    blu blu\n"
                "    ok\n";

        QTest::newRow("cascade 1")
            <<  "bla bla\n"
                "    blu blu\n"
            << std::make_pair(1, 11)
            << "\nok"
            <<  "bla bla\n"
                "    blu blu\n"
                "    ok\n";

        QTest::newRow("empty line 2")
            <<  "    empty line padded with 4 spcs\n"
                "    \n"
            << std::make_pair(1, 4)
            << "\nok"
            <<  "    empty line padded with 4 spcs\n"
                "    \n"
                "    ok\n";

        QTest::newRow("midbreak")
            << "    bla bla    blu blu\n"
            << std::make_pair(0, 11)
            << "\n"
            <<  "    bla bla\n"
                "    blu blu\n";

        QTest::newRow("midbreak 2")
            <<  "    bla bla blu blu\n"
            << std::make_pair(0, 11)
            <<  "\n"
            <<  "    bla bla\n"
                "    blu blu\n";

        QTest::newRow("normal 1")
            << "bla bla\n"
            << std::make_pair(0, 7)
            << "\nok"
            << "bla bla\n"
               "ok\n";

        QTest::newRow("newline")
            <<  "    sadf\n"
                "\n"
            << std::make_pair(1, 0)
            << "\nok"
            <<  "    sadf\n"
                "\n"
                "ok\n";
    }

    void normal() {
        runDataDrivenTest();
    }

    void lua_data() {
        addColumns();
        QTest::newRow("lua")
            << "    bla bla\n"
            << std::make_pair(0, 11)
            << "\nok"
            << "    bla bla\n"
               "    ok\n";
    }

    void lua() {
        qpart.initHighlighter("file.lua");
        runDataDrivenTest();
    }
};


QTEST_MAIN(Test)
#include "indenter.moc"
