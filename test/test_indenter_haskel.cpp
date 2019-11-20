
    void Test() {
        qpart.initHighlighter("file.TODO");
        runDataDrivenTest();
    }

    void Test_data() {
        addColumns();

        QTest::newRow("dontIndent1")
            <<  "main = do\n"
                "    -- This is a comment\n"
                "    something\n"
                "    something\n"
                "\n"
                "\"\"]"
            <<  std::make_pair(0, 0)
            <<  "\n"
                "foo bar"
            <<  "main = do\n"
                "    -- This is a comment\n"
                "    something\n"
                "    something\n"
                "\n"
                "    foo bar\n"
                "\"\"]\n"
                "";

        QTest::newRow("if2")
            <<  "foo = if true\n"
                "\"         then 1\"]"
            <<  std::make_pair(0, 0)
            <<  "\n"
                "else 2"
            <<  "foo = if true\n"
                "         then 1\n"
                "\"         else 2\"]\n"
                "";

        QTest::newRow("afterComma1")
            <<  "primitives = [(\"+\", numericBinop (+)),\n"
                "\"\"]"
            <<  std::make_pair(0, 0)
            <<  "\n"
                "(\\"-\\", numericBinop (-)),"
            <<  "primitives = [(\"+\", numericBinop (+)),\n"
                "    (\"-\", numericBinop (-)),\n"
                "\"\"]\n"
                "";

        QTest::newRow("parsec1")
            <<  "parseExpr = parseString\n"
                "        <|> parseNumber\n"
                "\"\"]"
            <<  std::make_pair(0, 0)
            <<  "\n"
                "<|> parseAtom"
            <<  "parseExpr = parseString\n"
                "        <|> parseNumber\n"
                "        <|> parseAtom\n"
                "\"\"]\n"
                "";

        QTest::newRow("if1")
            <<  "foo = if true\n"
                "\"\"]"
            <<  std::make_pair(0, 0)
            <<  "\n"
                "then"
            <<  "foo = if true\n"
                "         then\n"
                "\"\"]\n"
                "";
    }
