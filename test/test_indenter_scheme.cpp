#include <QtTest/QtTest>

#include "base_test.h"


class Test: public BaseTest
{
    Q_OBJECT

private slots:
    void test() {
        // qpart.setIndentAlgorithm(Qutepart::INDENT_ALG_SCHEME);
        runDataDrivenTest();
    }

    void test_data() {
        addColumns();

        QTest::newRow("1")
            <<  "(myfunc a"
            <<  std::make_pair(0, 0)
            <<  "\n"
                "b"
            <<  "(myfunc a\n"
                "        b";

        QTest::newRow("2")
            <<  "(myfunc (if (a) a b)"
            <<  std::make_pair(0, 0)
            <<  "\n"
                "b"
            <<  "(myfunc (if (a) a b)\n"
                "        b";

        QTest::newRow("3")
            <<  "(myfunc a)"
            <<  std::make_pair(0, 0)
            <<  "\n"
                "b"
            <<  "(myfunc a)\n"
                "b";

        QTest::newRow("4")
            <<  "  (myfunc a)"
            <<  std::make_pair(0, 0)
            <<  "\n"
                "b"
            <<  "  (myfunc a)\n"
                "  b";

        QTest::newRow("5")
            <<  "    (define"
            <<  std::make_pair(0, 0)
            <<  "\n"
                "b"
            <<  "    (define\n"
                "     b";

        QTest::newRow("6")
            <<  "a\n"
                "b"
            <<  std::make_pair(0, 0)
            <<  "\n"
                "x"
            <<  "a\n"
                "b\n"
                "x";

        QTest::newRow("7")
            <<  "   a\n"
                "'']"
            <<  std::make_pair(0, 0)
            <<  "\n"
                "x"
            <<  "   a\n"
                "\n"
                "   x";

        QTest::newRow("8")
            <<  "(define myfunc"
            <<  std::make_pair(0, 0)
            <<  "\n"
                "x"
            <<  "(define myfunc\n"
                "  x";

        QTest::newRow("9")
            <<  "(let ((pi 3.14) (r 120))"
            <<  std::make_pair(0, 0)
            <<  "\n"
                "x"
            <<  "(let ((pi 3.14) (r 120))\n"
                "  x";

        QTest::newRow("1")
            <<  "(define (fac n)\n"
                "(if (zero? n)\n"
                "1\n"
                "(* n (fac (- n 1)))))"
            <<  std::make_pair(0, 0)
            <<  "<<align all>>"
            <<  "(define (fac n)\n"
                "  (if (zero? n)\n"
                "      1\n"
                "      (* n (fac (- n 1)))))";

        QTest::newRow("2")
            <<  "(let ((fnord 5)\n"
                "(answer 42))\n"
                "(frobnicate fnord answer))"
            <<  std::make_pair(0, 0)
            <<  "<<align all>>"
            <<  "(let ((fnord 5)\n"
                "      (answer 42))\n"
                "  (frobnicate fnord answer))";

        QTest::newRow("3")
            <<  "(list (foo)\n"
                "(bar)\n"
                "(baz))"
            <<  std::make_pair(0, 0)
            <<  "<<align all>>"
            <<  "(list (foo)\n"
                "      (bar)\n"
                "      (baz))";

        QTest::newRow("4")
            <<  "(list\n"
                "(foo)\n"
                "(bar)\n"
                "(baz))"
            <<  std::make_pair(0, 0)
            <<  "<<align all>>"
            <<  "(list\n"
                " (foo)\n"
                " (bar)\n"
                " (baz))";

        QTest::newRow("5")
            <<  "(let ((pi 3.14)\n"
                "(r 120))\n"
                "(* pi r r))"
            <<  std::make_pair(0, 0)
            <<  "<<align all>>"
            <<  "(let ((pi 3.14)\n"
                "      (r 120))\n"
                "  (* pi r r))";

        QTest::newRow("6")
            <<  "(cond\n"
                "((good? x) (handle-good x))\n"
                "((bad? x)  (handle-bad x))\n"
                "((ugly? x) (handle-ugly x))\n"
                "(else      (handle-default x)))"
            <<  std::make_pair(0, 0)
            <<  "<<align all>>"
            <<  "(cond\n"
                " ((good? x) (handle-good x))\n"
                " ((bad? x)  (handle-bad x))\n"
                " ((ugly? x) (handle-ugly x))\n"
                " (else      (handle-default x)))";

        QTest::newRow("7")
            <<  "(cond\n"
                "((good? x)\n"
                "(handle-good x))\n"
                "((bad? x)\n"
                "(handle-bad (if (really-bad? x)\n"
                "(really-bad->bad x)\n"
                "x)))\n"
                "((ugly? x)\n"
                "(handle-ugly x))\n"
                "(else\n"
                "(handle-default x)))"
            <<  std::make_pair(0, 0)
            <<  "<<align all>>"
            <<  "(cond\n"
                " ((good? x)\n"
                "  (handle-good x))\n"
                " ((bad? x)\n"
                "  (handle-bad (if (really-bad? x)\n"
                "                  (really-bad->bad x)\n"
                "                  x)))\n"
                " ((ugly? x)\n"
                "  (handle-ugly x))\n"
                " (else\n"
                "  (handle-default x)))";

        QTest::newRow("8")
            <<  "(module gauss mzscheme\n"
                "\n"
                "(define (sum-up-to n)\n"
                "(/ (* n (+ n 1))\n"
                "2))\n"
                "\n"
                "(define (sum-up-to n)\n"
                "(/ (* n (+ n 1))\n"
                "2))\n"
                "\n"
                ")"
            <<  std::make_pair(0, 0)
            <<  "<<align all>>"
            <<  "(module gauss mzscheme\n"
                "\n"
                "(define (sum-up-to n)\n"
                "  (/ (* n (+ n 1))\n"
                "     2))\n"
                "\n"
                "(define (sum-up-to n)\n"
                "  (/ (* n (+ n 1))\n"
                "     2))\n"
                "\n"
                ")";
    }
};


QTEST_MAIN(Test)
#include "test_indenter_scheme.moc"
