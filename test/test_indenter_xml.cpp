
    void BaseTestClass() {
        qpart.initHighlighter("file.TODO");
        runDataDrivenTest();
    }

    void BaseTestClass_data() {
        addColumns();
    }

    void Split() {
        qpart.initHighlighter("file.TODO");
        runDataDrivenTest();
    }

    void Split_data() {
        addColumns();

        QTest::newRow("split1")
            <<  "<one><two>\n"
                "'']"
            <<  std::make_pair(0, 0)
            <<  "<<alignLine(0)>>"
            <<  "<one>\n"
                "  <two>\n"
                "'']\n"
                "";

        QTest::newRow("split2")
            <<  "<one><two> text </two></one>\n"
                "'']"
            <<  std::make_pair(0, 0)
            <<  "<<alignLine(0)>>"
            <<  "<one>\n"
                "  <two> text </two>\n"
                "</one>\n"
                "'']\n"
                "";

        QTest::newRow("split3")
            <<  "<property name=\"geometry\">\n"
                "<rect>\n"
                "<x>0</x><y>0</y><width>421</width><height>300</height>\n"
                "</rect>\n"
                "</property>"
            <<  std::make_pair(0, 0)
            <<  "<<alignLine(i)>>"
            <<  "<property name=\"geometry\">\n"
                "  <rect>\n"
                "    <x>0</x>\n"
                "    <y>0</y>\n"
                "    <width>421</width>\n"
                "    <height>300</height>\n"
                "  </rect>\n"
                "</property>";

        QTest::newRow("split4")
            <<  "<a><b>8</b></a>"
            <<  std::make_pair(0, 0)
            <<  "<<alignLine(0)>>"
            <<  "<a>\n"
                "  <b>8</b>\n"
                "'</a>']\n"
                "";

        QTest::newRow("align1")
            <<  "<one>\n"
                "<two>"
            <<  std::make_pair(0, 0)
            <<  "<<alignLine(1)>>"
            <<  "<one>\n"
                "  <two>";

        QTest::newRow("align2")
            <<  "    text\n"
                "    </two>"
            <<  std::make_pair(0, 0)
            <<  "<<alignLine(1)>>"
            <<  "    text\n"
                "  </two>";

        QTest::newRow("align3")
            <<  "  <!-- blabla -->\n"
                "'      <tag>']"
            <<  std::make_pair(0, 0)
            <<  "<<alignLine(1)>>"
            <<  "  <!-- blabla -->\n"
                "'  <tag>']\n"
                "";

        QTest::newRow("align4")
            <<  "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
            <<  std::make_pair(0, 0)
            <<  "<<alignLine(0)>>"
            <<  "<?xml version=\"1.0\" encoding=\"UTF-8\"?>";

        QTest::newRow("align5")
            <<  "<a>\n"
                "<b>"
            <<  std::make_pair(0, 0)
            <<  "<<alignLine(1)>>"
            <<  "<a>\n"
                "  <b>";

        QTest::newRow("slash1")
            <<  "    text\n"
                "    <\n"
                "'']"
            <<  std::make_pair(0, 0)
            <<  "/"
            <<  "    text\n"
                "  </\n"
                "'']\n"
                "";

        QTest::newRow("slash2")
            <<  "    text\n"
                "    <tag text=\"\n"
                "'']"
            <<  std::make_pair(0, 0)
            <<  "/"
            <<  "    text\n"
                "    <tag text=\"/\n"
                "'']\n"
                "";

        QTest::newRow("slash3")
            <<  "    <tag>\n"
                "    <\n"
                "'']"
            <<  std::make_pair(0, 0)
            <<  "/"
            <<  "    <tag>\n"
                "    </\n"
                "'']\n"
                "";

        QTest::newRow("slash4")
            <<  "    text\n"
                "    <blabla\n"
                "'']"
            <<  std::make_pair(0, 0)
            <<  "/"
            <<  "    text\n"
                "    </blabla\n"
                "'']\n"
                "";

        QTest::newRow("greater1")
            <<  "    text\n"
                "    </tag\n"
                "'']"
            <<  std::make_pair(0, 0)
            <<  ">"
            <<  "    text\n"
                "  </tag>\n"
                "'']\n"
                "";

        QTest::newRow("grater2")
            <<  "    <tag>\n"
                "    </tag\n"
                "'']"
            <<  std::make_pair(0, 0)
            <<  ">"
            <<  "    <tag>\n"
                "    </tag>\n"
                "'']\n"
                "";

        QTest::newRow("greater3")
            <<  "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
                "<tag\n"
                "'']"
            <<  std::make_pair(0, 0)
            <<  ">"
            <<  "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
                "<tag>\n"
                "'']\n"
                "";

        QTest::newRow("greater4")
            <<  "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd>\n"
                "<tag\n"
                "'']"
            <<  std::make_pair(0, 0)
            <<  ">"
            <<  "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd>\n"
                "<tag>\n"
                "'']\n"
                "";

        QTest::newRow("greater5")
            <<  "  <!-- blabla -->\n"
                "  <tag\n"
                "'']"
            <<  std::make_pair(0, 0)
            <<  ">"
            <<  "  <!-- blabla -->\n"
                "  <tag>\n"
                "'']\n"
                "";

        QTest::newRow("greater6")
            <<  "  </tag>\n"
                "  <tag\n"
                "'']"
            <<  std::make_pair(0, 0)
            <<  ">"
            <<  "  </tag>\n"
                "  <tag>\n"
                "'']\n"
                "";

        QTest::newRow("greater7")
            <<  "  <tag>\n"
                "  <othertag\n"
                "'']"
            <<  std::make_pair(0, 0)
            <<  ">"
            <<  "  <tag>\n"
                "    <othertag>\n"
                "'']\n"
                "";

        QTest::newRow("enter1")
            <<  "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
                "'']"
            <<  std::make_pair(0, 0)
            <<  "\n"
                ""
            <<  "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
                "\n"
                "'']\n"
                "";

        QTest::newRow("enter2")
            <<  "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd>\n"
                "'']"
            <<  std::make_pair(0, 0)
            <<  "\n"
                ""
            <<  "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd>\n"
                "\n"
                "'']\n"
                "";

        QTest::newRow("enter3")
            <<  "  <!-- blabla -->\n"
                "'']"
            <<  std::make_pair(0, 0)
            <<  "\n"
                ""
            <<  "  <!-- blabla -->\n"
                "  \n"
                "'']\n"
                "";

        QTest::newRow("enter4")
            <<  "  </tag>\n"
                "'']"
            <<  std::make_pair(0, 0)
            <<  "\n"
                ""
            <<  "  </tag>\n"
                "  \n"
                "'']\n"
                "";

        QTest::newRow("enter5")
            <<  "  <tag>\n"
                "'']"
            <<  std::make_pair(0, 0)
            <<  "\n"
                ""
            <<  "  <tag>\n"
                "    \n"
                "'']\n"
                "";
    }
