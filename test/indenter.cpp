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

    void lisp_data() {
        addColumns();
        QTest::newRow("three semicolons")
            <<  "      \n"
                "   asdf"
            << std::make_pair(0, 6)
            << ";;;"
            <<  ";;;"
                "   asdf";

        QTest::newRow("two semicolons")
            <<  "      \n"
                "   asdf"
            <<  std::make_pair(0, 6)
            <<  ";;"
            <<  "   ;;";

        QTest::newRow("find brace")
            <<  "  (bla                   (x (y (z)))"
            <<  std::make_pair(0, 6)
            <<  "\n"
            <<  "  (bla                   (x (y (z)))\n"
                "    ";

        QTest::newRow("not found brace")
            <<  "  (bla                   (x (y (z))))"
            <<  std::make_pair(0, 37)
            <<  "\n"
            <<  "  (bla                   (x (y (z))))\n";
    }

    void lisp() {
        qpart.initHighlighter("file.lisp");
        runDataDrivenTest();
    }

    void python_data() {
        addColumns();
        QTest::newRow("dedent return")
            <<  "def some_function():\n"
                "  return"
            <<  std::make_pair(1, 11)
            <<  "\npass"
            <<  "def some_function():\n"
                "  return\n"
                "pass";

        QTest::newRow("dedent continue")
            <<  "while True:\n"
                "  continue"
            <<  std::make_pair(1, 11)
            <<  "\npass"
            <<  "while True:\n"
                "  continue\n"
                "pass";

        QTest::newRow("keep indent 2")
            <<  "class my_class():\n"
                "  def my_fun():\n"
                "    print \"Foo\"\n"
                "    print 3"
            <<  std::make_pair(3, 12)
            <<  "\npass"
            <<  "class my_class():\n"
                "  def my_fun():\n"
                "    print \"Foo\"\n"
                "    print 3\n"
                "    pass";

        QTest::newRow("keep indent 4")
            <<  "def some_function():"
            <<  std::make_pair(0, 22)
            <<  "\npass\n\npass"
            <<  "def some_function():\n"
                "  pass\n"
                "\n"
                "pass";

        QTest::newRow("dedent raise")
            <<  "try:\n"
                "  raise"
            <<  std::make_pair(1, 9)
            <<  "\nexcept:"
            <<  "try:\n"
                "  raise\n"
                "except:";

        QTest::newRow("indent colon 1")
            <<  "def some_function(param, param2):"
            <<  std::make_pair(0, 34)
            <<  "\npass"
            <<  "def some_function(param, param2):\n"
                "  pass";

        QTest::newRow("indent colon 2")
            <<  "def some_function(1,\n"
                "                  2):"
            <<  std::make_pair(1, 21)
            <<  "\npass"
            <<  "def some_function(1,\n"
                "                  2):\n"
                "  pass";

        QTest::newRow("indent colon 3")  // do not indent colon if hanging indentation used
            <<  "    a = {1:"
            <<  std::make_pair(0, 12)
            <<  "\nx"
            <<  "    a = {1:\n"
                "         x";

        QTest::newRow("dedent pass")
            <<  "def some_function():\n"
                "  pass"
            <<  std::make_pair(1, 8)
            <<  "\npass"
            <<  "def some_function():\n"
                "  pass\n"
                "pass";

        QTest::newRow("dedent return")
            <<  "def some_function():\n"
                "  return"
            <<  std::make_pair(1, 11)
            <<  "\npass"
            <<  "def some_function():\n"
                "  return\n"
                "pass";

        QTest::newRow("keep indent 3")
            <<  "while True:\n"
                "  returnFunc()\n"
                "  myVar = 3"
            <<  std::make_pair(2, 12)
            <<  "\npass"
            <<  "while True:\n"
                "  returnFunc()\n"
                "  myVar = 3\n"
                "  pass";

        QTest::newRow("keep indent 1")
            <<  "def some_function(param, param2):\n"
                "  a = 5\n"
                "  b = 7"
            <<  std::make_pair(2, 8)
            <<  "\npass"
            <<  "def some_function(param, param2):\n"
                "  a = 5\n"
                "  b = 7\n"
                "  pass";

        QTest::newRow("autoindent after empty")
            <<  "while True:\n"
                "   returnFunc()\n"
                "\n"
                "   myVar = 3"
            <<  std::make_pair(2, 0)
            <<  "\n\tx"
            <<  "while True:\n"
                "   returnFunc()\n"
                "\n"
                "   x\n"
                "   myVar = 3";

        QTest::newRow("hanging indentation")
            <<  "     return func (something,"
            <<  std::make_pair(0, 28)
            <<  "\nx"
            <<  "     return func (something,\n"
                "                  x";

        QTest::newRow("hanging indentation 2")
            <<  "     return func (\n"
                "        something,"
            <<  std::make_pair(1, 19)
            <<  "\nx"
            <<  "     return func (\n"
                "        something,\n"
                "        x";

        QTest::newRow("hanging indentation 3")
            <<  "      a = func (\n"
                "          something)"
            <<  std::make_pair(1, 19)
            <<  "\nx"
            <<  "      a = func (\n"
                "          something)"
                "      x";

        QTest::newRow("hanging indentation 4")
            <<  "     return func(a,\n"
                "                 another_func(1,\n"
                "                              2),"
            <<  std::make_pair(2, 33)
            <<  "\nx"
            <<  "     return func(a,\n"
                "                 another_func(1,\n"
                "                              2),\n"
                "                 x";

        QTest::newRow("hanging indentation 5")
            <<  "     return func(another_func(1,\n"
                "                              2),"
            <<  std::make_pair(2, 33)
            <<  "\nx"
            <<  "     return func(another_func(1,\n"
                "                              2),\n"
                "                 x";
    }

    void python() {
        qpart.initHighlighter("file.py");
        runDataDrivenTest();
    }
};


QTEST_MAIN(Test)
#include "indenter.moc"
