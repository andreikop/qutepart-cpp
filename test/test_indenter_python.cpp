#include <QtTest/QtTest>

#include "base_test.h"


class Test: public BaseTest
{
    Q_OBJECT

private slots:
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
        qpart.setHighlighter("python.xml");
        runDataDrivenTest();
    }
};


QTEST_MAIN(Test)
#include "test_indenter_python.moc"
