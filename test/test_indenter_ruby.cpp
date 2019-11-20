#include <QtTest/QtTest>

#include "base_test.h"


class Test: public BaseTest
{
    Q_OBJECT

private slots:
    void If() {
        qpart.initHighlighter("file.TODO");
        runDataDrivenTest();
    }

    void If_data() {
        addColumns();

        QTest::newRow("if10")
            <<  "  if foo\n"
                "\"\"]"
            <<  std::make_pair(0, 8)
            <<  ""
            <<  "  if foo\n"
                "    blah\n"
                "\"\"]";

        QTest::newRow("if11")
            <<  "  if foo\n"
                "    blah\n"
                "\"\"]"
            <<  std::make_pair(1, 8)
            <<  ""
            <<  "  if foo\n"
                "    blah\n"
                "  end\n"
                "\"\"]";

        QTest::newRow("if20")
            <<  "  var = if foo\n"
                "\"\"]"
            <<  std::make_pair(0, 14)
            <<  ""
            <<  "  var = if foo\n"
                "    blah\n"
                "\"\"]";

        QTest::newRow("if21")
            <<  "  var = if foo\n"
                "    blah\n"
                "\"\"]"
            <<  std::make_pair(1, 8)
            <<  ""
            <<  "  var = if foo\n"
                "    blah\n"
                "  end\n"
                "\"\"]";

        QTest::newRow("if22")
            <<  "  var = bar if foo\n"
                "\"\"]"
            <<  std::make_pair(0, 18)
            <<  ""
            <<  "  var = bar if foo\n"
                "  blah\n"
                "\"\"]";

        QTest::newRow("if30")
            <<  "  if foo; 42 else 37 end\n"
                "\"\"]"
            <<  std::make_pair(0, 24)
            <<  ""
            <<  "  if foo; 42 else 37 end\n"
                "  blah\n"
                "\"\"]";

        QTest::newRow("if31")
            <<  "  if foo then 42 else 37 end\n"
                "\"\"]"
            <<  std::make_pair(0, 28)
            <<  ""
            <<  "  if foo then 42 else 37 end\n"
                "  blah\n"
                "\"\"]";

        QTest::newRow("block01")
            <<  "10.times {\n"
                "  foo\n"
                "\"\"]"
            <<  std::make_pair(1, 6)
            <<  ""
            <<  "10.times {\n"
                "  foo\n"
                "  blah\n"
                "\"\"]";

        QTest::newRow("block02")
            <<  "10.times {\n"
                "  if foo\n"
                "\"\"]"
            <<  std::make_pair(1, 8)
            <<  ""
            <<  "10.times {\n"
                "  if foo\n"
                "    blah\n"
                "\"\"]";

        QTest::newRow("basic1")
            <<  "# basic1.txt\n"
                "\"\"]"
            <<  std::make_pair(0, 12)
            <<  ""
            <<  "# basic1.txt\n"
                "def foo\n"
                "  if gets\n"
                "    puts\n"
                "  else\n"
                "    exit\n"
                "  end\n"
                "end\n"
                "\"\"]";

        QTest::newRow("basic2")
            <<  "# basic2.txt\n"
                "\n"
                "\"\"]"
            <<  std::make_pair(1, 0)
            <<  ""
            <<  "# basic2.txt\n"
                "\n"
                "class MyClass\n"
                "\n"
                "  attr_reader :foo\n"
                "\n"
                "  private\n"
                "\n"
                "  def helper(str)";

        QTest::newRow("basic3")
            <<  "def foo\n"
                "    if check\n"
                "       bar\n"
                "\"\"]"
            <<  std::make_pair(2, 10)
            <<  ""
            <<  "def foo\n"
                "    if check\n"
                "       bar\n"
                "    end\n"
                "end\n"
                "\"\"]";

        QTest::newRow("basic4")
            <<  "def foo\n"
                "    array.each do |v|\n"
                "       bar\n"
                "\"\"]"
            <<  std::make_pair(2, 21)
            <<  ""
            <<  "def foo\n"
                "    array.each do |v|\n"
                "       bar\n"
                "    end\n"
                "end\n"
                "\"\"]";

        QTest::newRow("empty_file1")
            <<  "\n"
                "\"\"]"
            <<  std::make_pair(0, 0)
            <<  ""
            <<  "\n"
                "\n"
                "# Comment\n"
                "def foo\n"
                "  bar\n"
                "end\n"
                "\"\"]";

        QTest::newRow("regexp1")
            <<  "  rx =~ /^hello/\n"
                "\"\"]"
            <<  std::make_pair(0, 16)
            <<  ""
            <<  "  rx =~ /^hello/\n"
                "  foo\n"
                "\"\"]";

        QTest::newRow("do1")
            <<  "# do1.txt\n"
                "5.times do\n"
                "\"\"]"
            <<  std::make_pair(1, 24)
            <<  ""
            <<  "# do1.txt\n"
                "5.times do\n"
                "end\n"
                "\"\"]";

        QTest::newRow("do2")
            <<  "# do2.txt"
            <<  std::make_pair(1, 24)
            <<  ""
            <<  "# do2.txt";

        QTest::newRow("do3")
            <<  "# do3.txt\n"
                "[1,2,3].each_with_index do |obj, i|\n"
                "\"\"]"
            <<  std::make_pair(1, 35)
            <<  ""
            <<  "# do3.txt\n"
                "[1,2,3].each_with_index do |obj, i|";

        QTest::newRow("do4")
            <<  "# do4.txt"
            <<  std::make_pair(1, 31)
            <<  ""
            <<  "# do4.txt";

        QTest::newRow("do5")
            <<  "def foo(f)\n"
                "  f.each do # loop\n"
                "\"\"]"
            <<  std::make_pair(1, 18)
            <<  ""
            <<  "def foo(f)\n"
                "  f.each do # loop\n"
                "  end\n"
                "\"\"]";

        QTest::newRow("do6")
            <<  "def foo(f)\n"
                "  f.each do # loop\n"
                "\"\"]"
            <<  std::make_pair(1, 18)
            <<  ""
            <<  "def foo(f)\n"
                "  f.each do # loop\n"
                "    bar\n"
                "  end\n"
                "\"\"]";

        QTest::newRow("do7")
            <<  "def foo(f)\n"
                "  f.each do # loop with do\n"
                "\"\"]"
            <<  std::make_pair(1, 26)
            <<  ""
            <<  "def foo(f)\n"
                "  f.each do # loop with do\n"
                "    bar\n"
                "  end\n"
                "\"\"]";

        QTest::newRow("no_do1")
            <<  "# no-do1.txt\n"
                "if foo\n"
                "  # nothing to do\n"
                "\"\"]"
            <<  std::make_pair(2, 32)
            <<  ""
            <<  "# no-do1.txt\n"
                "if foo\n"
                "  # nothing to do\n"
                "end\n"
                "\"\"]";

        QTest::newRow("no_do2")
            <<  "# no-do2.txt\n"
                "if foo\n"
                "  # nothing to do\n"
                "\"\"]"
            <<  std::make_pair(2, 32)
            <<  ""
            <<  "# no-do2.txt\n"
                "if foo\n"
                "  # nothing to do\n"
                "  f\n"
                "end\n"
                "\"\"]";

        QTest::newRow("no_do3")
            <<  "# no-do3.txt\n"
                "if foo"
            <<  std::make_pair(2, 32)
            <<  ""
            <<  "# no-do3.txt\n"
                "if foo";

        QTest::newRow("no_do4")
            <<  "# no-do4.txt\n"
                "if foo"
            <<  std::make_pair(2, 32)
            <<  ""
            <<  "# no-do4.txt\n"
                "if foo";

        QTest::newRow("singleline01")
            <<  "  def foo() 42 end\n"
                "\"\"]"
            <<  std::make_pair(0, 18)
            <<  ""
            <<  "  def foo() 42 end\n"
                "  blah\n"
                "\"\"]";

        QTest::newRow("singleline02")
            <<  "  def foo; 42 end\n"
                "\"\"]"
            <<  std::make_pair(0, 17)
            <<  ""
            <<  "  def foo; 42 end\n"
                "  blah\n"
                "\"\"]";

        QTest::newRow("singleline03")
            <<  "  def foo() bar\n"
                "\"\"]"
            <<  std::make_pair(0, 15)
            <<  ""
            <<  "  def foo() bar\n"
                "    blah\n"
                "\"\"]";

        QTest::newRow("singleline04")
            <<  "  def foo; bar\n"
                "    blah\n"
                "\"\"]"
            <<  std::make_pair(1, 8)
            <<  ""
            <<  "  def foo; bar\n"
                "    blah\n"
                "  end\n"
                "\"\"]";

        QTest::newRow("array1")
            <<  "  array = [ :a, :b, :c ]\n"
                "\"\"]"
            <<  std::make_pair(0, 24)
            <<  ""
            <<  "  array = [ :a, :b, :c ]\n"
                "  foo\n"
                "\"\"]";

        QTest::newRow("array2")
            <<  "  array = [\n"
                "\"\"]"
            <<  std::make_pair(0, 11)
            <<  ""
            <<  "  array = [\n"
                "    :a\n"
                "\"\"]";

        QTest::newRow("array3")
            <<  "  array = [\n"
                "    :a,\n"
                "\"\"]"
            <<  std::make_pair(1, 7)
            <<  ""
            <<  "  array = [\n"
                "    :a,\n"
                "    :b\n"
                "\"\"]";

        QTest::newRow("array4")
            <<  "  array = [\n"
                "    :a,\n"
                "    :b\n"
                "\"\"]"
            <<  std::make_pair(2, 6)
            <<  ""
            <<  "  array = [\n"
                "    :a,\n"
                "    :b\n"
                "  ]\n"
                "\"\"]";

        QTest::newRow("array5")
            <<  "  array = [\n"
                "    :a,\n"
                "    :b\n"
                "\"\"]"
            <<  std::make_pair(2, 6)
            <<  ""
            <<  "  array = [\n"
                "    :a,\n"
                "    :b\n"
                "  (3,2)\n"
                "\n"
                "\"\"]";

        QTest::newRow("array6")
            <<  "  array = [:a,\n"
                "           :b,\n"
                "           :c]\n"
                "\"\"]"
            <<  std::make_pair(2, 14)
            <<  ""
            <<  "  array = [:a,\n"
                "           :b,\n"
                "           :c]\n"
                "  foo\n"
                "\"\"]";

        QTest::newRow("array7")
            <<  "  array = [:a,\n"
                "\"\"]"
            <<  std::make_pair(0, 14)
            <<  ""
            <<  "  array = [:a,\n"
                "           :b\n"
                "\"\"]";

        QTest::newRow("array8")
            <<  "  array = [:a,\n"
                "           :b,\n"
                "\"\"]"
            <<  std::make_pair(1, 14)
            <<  ""
            <<  "  array = [:a,\n"
                "           :b,\n"
                "           :c\n"
                "\"\"]";

        QTest::newRow("array9")
            <<  "  array = [:a,\n"
                "           :b,\n"
                "           :c\n"
                "\"\"]"
            <<  std::make_pair(2, 13)
            <<  ""
            <<  "  array = [:a,\n"
                "           :b,\n"
                "           :c]\n"
                "  foo\n"
                "\"\"]";

        QTest::newRow("array10")
            <<  "  array = [:a,\n"
                "           :b,\n"
                "           [:foo,\n"
                "\"\"]"
            <<  std::make_pair(2, 17)
            <<  ""
            <<  "  array = [:a,\n"
                "           :b,\n"
                "           [:foo,\n"
                "            :bar\n"
                "\"\"]";

        QTest::newRow("array11")
            <<  "  array = [:a,\n"
                "           :b,\n"
                "           [:foo,\n"
                "            :bar,\n"
                "\"\"]"
            <<  std::make_pair(3, 17)
            <<  ""
            <<  "  array = [:a,\n"
                "           :b,\n"
                "           [:foo,\n"
                "            :bar,\n"
                "            :baz\n"
                "\"\"]";

        QTest::newRow("array12")
            <<  "  array = [:a,\n"
                "           :b,\n"
                "           [:foo,\n"
                "            :bar],\n"
                "\"\"]"
            <<  std::make_pair(3, 18)
            <<  ""
            <<  "  array = [:a,\n"
                "           :b,\n"
                "           [:foo,\n"
                "            :bar],\n"
                "           :baz\n"
                "\"\"]";

        QTest::newRow("array16")
            <<  "  array = [ :a,\n"
                "            :b,\n"
                "            :c ]\n"
                "\"\"]"
            <<  std::make_pair(2, 16)
            <<  ""
            <<  "  array = [ :a,\n"
                "            :b,\n"
                "            :c ]\n"
                "  foo\n"
                "\"\"]";

        QTest::newRow("array17")
            <<  "  array = [ :a,\n"
                "\"\"]"
            <<  std::make_pair(0, 15)
            <<  ""
            <<  "  array = [ :a,\n"
                "            :b\n"
                "\"\"]";

        QTest::newRow("array18")
            <<  "  array = [ :a,\n"
                "            :b,\n"
                "\"\"]"
            <<  std::make_pair(1, 15)
            <<  ""
            <<  "  array = [ :a,\n"
                "            :b,\n"
                "            :c\n"
                "\"\"]";

        QTest::newRow("array19")
            <<  "  array = [ :a,\n"
                "            :b,\n"
                "            :c\n"
                "\"\"]"
            <<  std::make_pair(2, 14)
            <<  ""
            <<  "  array = [ :a,\n"
                "            :b,\n"
                "            :c ]\n"
                "  foo\n"
                "\"\"]";

        QTest::newRow("array20")
            <<  "  array = [ :a,\n"
                "            :b,\n"
                "            [ :foo,\n"
                "\"\"]"
            <<  std::make_pair(2, 19)
            <<  ""
            <<  "  array = [ :a,\n"
                "            :b,\n"
                "            [ :foo,\n"
                "              :bar\n"
                "\"\"]";

        QTest::newRow("array21")
            <<  "  array = [ :a,\n"
                "            :b,\n"
                "            [ :foo,\n"
                "              :bar,\n"
                "\"\"]"
            <<  std::make_pair(3, 19)
            <<  ""
            <<  "  array = [ :a,\n"
                "            :b,\n"
                "            [ :foo,\n"
                "              :bar,\n"
                "              :baz\n"
                "\"\"]";

        QTest::newRow("array22")
            <<  "  array = [ :a,\n"
                "            :b,\n"
                "            [ :foo,\n"
                "              :bar ],\n"
                "\"\"]"
            <<  std::make_pair(3, 21)
            <<  ""
            <<  "  array = [ :a,\n"
                "            :b,\n"
                "            [ :foo,\n"
                "              :bar ],\n"
                "            :baz\n"
                "\"\"]";

        QTest::newRow("array_comment1")
            <<  "  array = [ :a, :b, :c ] # comment\n"
                "                         # comment\n"
                "\"\"]"
            <<  std::make_pair(1, 34)
            <<  ""
            <<  "  array = [ :a, :b, :c ] # comment\n"
                "                         # comment\n"
                "  foo\n"
                "\"\"]";

        QTest::newRow("array_comment2")
            <<  "  array = [ # comment\n"
                "\"\"]"
            <<  std::make_pair(0, 21)
            <<  ""
            <<  "  array = [ # comment\n"
                "    :a\n"
                "\"\"]";

        QTest::newRow("array_comment3")
            <<  "  array = [ # comment\n"
                "    :a,     # comment\n"
                "\"\"]"
            <<  std::make_pair(1, 21)
            <<  ""
            <<  "  array = [ # comment\n"
                "    :a,     # comment\n"
                "    :b\n"
                "\"\"]";

        QTest::newRow("array_comment4")
            <<  "  array = [\n"
                "    :a,\n"
                "    :b # comment,\n"
                "\"\"]"
            <<  std::make_pair(2, 17)
            <<  ""
            <<  "  array = [\n"
                "    :a,\n"
                "    :b # comment,\n"
                "  ]\n"
                "\"\"]";

        QTest::newRow("array_comment5")
            <<  "  array = [\n"
                "    :a,\n"
                "    :b # comment\n"
                "\"\"]"
            <<  std::make_pair(2, 16)
            <<  ""
            <<  "  array = [\n"
                "    :a,\n"
                "    :b # comment\n"
                "  (3,2)\n"
                "\n"
                "\"\"]";

        QTest::newRow("hash1")
            <<  "  hash = { :a => 1, :b => 2, :c => 3 }\n"
                "\"\"]"
            <<  std::make_pair(0, 38)
            <<  ""
            <<  "  hash = { :a => 1, :b => 2, :c => 3 }\n"
                "  foo\n"
                "\"\"]";

        QTest::newRow("hash2")
            <<  "  hash = {\n"
                "\"\"]"
            <<  std::make_pair(0, 10)
            <<  ""
            <<  "  hash = {\n"
                "    :a => 1\n"
                "\"\"]";

        QTest::newRow("hash3")
            <<  "  hash = {\n"
                "    :a => 1,\n"
                "\"\"]"
            <<  std::make_pair(1, 12)
            <<  ""
            <<  "  hash = {\n"
                "    :a => 1,\n"
                "    :b => 2\n"
                "\"\"]";

        QTest::newRow("hash4")
            <<  "  hash = {\n"
                "    :a => 1,\n"
                "    :b => 2\n"
                "\"\"]"
            <<  std::make_pair(2, 11)
            <<  ""
            <<  "  hash = {\n"
                "    :a => 1,\n"
                "    :b => 2\n"
                "  }\n"
                "\"\"]";

        QTest::newRow("hash5")
            <<  "  hash = {\n"
                "    :a => 1,\n"
                "    :b => 2\n"
                "\"\"]"
            <<  std::make_pair(2, 11)
            <<  ""
            <<  "  hash = {\n"
                "    :a => 1,\n"
                "    :b => 2\n"
                "  (3,2)\n"
                "\n"
                "\"\"]";

        QTest::newRow("hash6")
            <<  "  hash = {:a => 1,\n"
                "          :b => 2,\n"
                "          :c => 3}\n"
                "\"\"]"
            <<  std::make_pair(2, 18)
            <<  ""
            <<  "  hash = {:a => 1,\n"
                "          :b => 2,\n"
                "          :c => 3}\n"
                "  foo\n"
                "\"\"]";

        QTest::newRow("hash7")
            <<  "  hash = {:a => 1,\n"
                "\"\"]"
            <<  std::make_pair(0, 18)
            <<  ""
            <<  "  hash = {:a => 1,\n"
                "          :b => 2\n"
                "\"\"]";

        QTest::newRow("hash8")
            <<  "  hash = {:a => 1,\n"
                "          :b => 2,\n"
                "\"\"]"
            <<  std::make_pair(1, 18)
            <<  ""
            <<  "  hash = {:a => 1,\n"
                "          :b => 2,\n"
                "          :c => 3\n"
                "\"\"]";

        QTest::newRow("hash9")
            <<  "  hash = {:a => 1,\n"
                "          :b => 2,\n"
                "          :c => 3\n"
                "\"\"]"
            <<  std::make_pair(2, 17)
            <<  ""
            <<  "  hash = {:a => 1,\n"
                "          :b => 2,\n"
                "          :c => 3}\n"
                "  foo\n"
                "\"\"]";

        QTest::newRow("hash10")
            <<  "  hash = {:a => 1,\n"
                "          :b => 2,\n"
                "          :c => {:foo => /^f/,\n"
                "\"\"]"
            <<  std::make_pair(2, 30)
            <<  ""
            <<  "  hash = {:a => 1,\n"
                "          :b => 2,\n"
                "          :c => {:foo => /^f/,\n"
                "                 :bar => /^b/\n"
                "\"\"]";

        QTest::newRow("hash11")
            <<  "  hash = {:a => 1,\n"
                "          :b => 2,\n"
                "          :c => {:foo => /^f/,\n"
                "                 :bar => /^b/},\n"
                "\"\"]"
            <<  std::make_pair(3, 31)
            <<  ""
            <<  "  hash = {:a => 1,\n"
                "          :b => 2,\n"
                "          :c => {:foo => /^f/,\n"
                "                 :bar => /^b/},\n"
                "          :d => 4\n"
                "\"\"]";

        QTest::newRow("hash12")
            <<  "  hash = {:a => 1,\n"
                "          :b => 2,\n"
                "          :c => {:foo => /^f/,\n"
                "                 :bar => /^b/,\n"
                "\"\"]"
            <<  std::make_pair(3, 30)
            <<  ""
            <<  "  hash = {:a => 1,\n"
                "          :b => 2,\n"
                "          :c => {:foo => /^f/,\n"
                "                 :bar => /^b/,\n"
                "                 :baz => /^b/\n"
                "\"\"]";

        QTest::newRow("hash16")
            <<  "  hash = { :a => 1,\n"
                "           :b => 2,\n"
                "           :c => 3 }\n"
                "\"\"]"
            <<  std::make_pair(2, 20)
            <<  ""
            <<  "  hash = { :a => 1,\n"
                "           :b => 2,\n"
                "           :c => 3 }\n"
                "  foo\n"
                "\"\"]";

        QTest::newRow("hash17")
            <<  "  hash = { :a => 1,\n"
                "\"\"]"
            <<  std::make_pair(0, 19)
            <<  ""
            <<  "  hash = { :a => 1,\n"
                "           :b => 2\n"
                "\"\"]";

        QTest::newRow("hash18")
            <<  "  hash = { :a => 1,\n"
                "           :b => 2,\n"
                "\"\"]"
            <<  std::make_pair(1, 19)
            <<  ""
            <<  "  hash = { :a => 1,\n"
                "           :b => 2,\n"
                "           :c => 3\n"
                "\"\"]";

        QTest::newRow("hash19")
            <<  "  hash = { :a => 1,\n"
                "           :b => 2,\n"
                "           :c => 3\n"
                "\"\"]"
            <<  std::make_pair(2, 18)
            <<  ""
            <<  "  hash = { :a => 1,\n"
                "           :b => 2,\n"
                "           :c => 3 }\n"
                "  foo\n"
                "\"\"]";

        QTest::newRow("hash20")
            <<  "  hash = { :a => 1,\n"
                "           :b => 2,\n"
                "           :c => { :foo => /^f/,\n"
                "\"\"]"
            <<  std::make_pair(2, 32)
            <<  ""
            <<  "  hash = { :a => 1,\n"
                "           :b => 2,\n"
                "           :c => { :foo => /^f/,\n"
                "                   :bar => /^b/\n"
                "\"\"]";

        QTest::newRow("hash21")
            <<  "  hash = { :a => 1,\n"
                "           :b => 2,\n"
                "           :c => { :foo => /^f/,\n"
                "                   :bar => /^b/ },\n"
                "\"\"]"
            <<  std::make_pair(3, 34)
            <<  ""
            <<  "  hash = { :a => 1,\n"
                "           :b => 2,\n"
                "           :c => { :foo => /^f/,\n"
                "                   :bar => /^b/ },\n"
                "           :d => 4\n"
                "\"\"]";

        QTest::newRow("hash22")
            <<  "  hash = { :a => 1,\n"
                "           :b => 2,\n"
                "           :c => { :foo => /^f/,\n"
                "                   :bar => /^b/,\n"
                "\"\"]"
            <<  std::make_pair(3, 32)
            <<  ""
            <<  "  hash = { :a => 1,\n"
                "           :b => 2,\n"
                "           :c => { :foo => /^f/,\n"
                "                   :bar => /^b/,\n"
                "                   :baz => /^b/\n"
                "\"\"]";

        QTest::newRow("ops1")
            <<  "t = foo() +\n"
                "\"\"]"
            <<  std::make_pair(0, 11)
            <<  ""
            <<  "t = foo() +\n"
                "    bar()\n"
                "\"\"]";

        QTest::newRow("ops2")
            <<  "t = foo() + # Comment\n"
                "\"\"]"
            <<  std::make_pair(0, 21)
            <<  ""
            <<  "t = foo() + # Comment\n"
                "    bar()\n"
                "\"\"]";

        QTest::newRow("ops3")
            <<  "t = foo() -\n"
                "\"\"]"
            <<  std::make_pair(0, 11)
            <<  ""
            <<  "t = foo() -\n"
                "    bar()\n"
                "\"\"]";

        QTest::newRow("ops4")
            <<  "t = foo() - # Comment\n"
                "\"\"]"
            <<  std::make_pair(0, 21)
            <<  ""
            <<  "t = foo() - # Comment\n"
                "    bar()\n"
                "\"\"]";

        QTest::newRow("ops5")
            <<  "t = foo() *\n"
                "\"\"]"
            <<  std::make_pair(0, 11)
            <<  ""
            <<  "t = foo() *\n"
                "    bar()\n"
                "\"\"]";

        QTest::newRow("ops6")
            <<  "t = foo() * # Comment\n"
                "\"\"]"
            <<  std::make_pair(0, 21)
            <<  ""
            <<  "t = foo() * # Comment\n"
                "    bar()\n"
                "\"\"]";

        QTest::newRow("ops7")
            <<  "t = foo() /\n"
                "\"\"]"
            <<  std::make_pair(0, 11)
            <<  ""
            <<  "t = foo() /\n"
                "    bar()\n"
                "\"\"]";

        QTest::newRow("ops8")
            <<  "t = foo() / # Comment\n"
                "\"\"]"
            <<  std::make_pair(0, 21)
            <<  ""
            <<  "t = foo() / # Comment\n"
                "    bar()\n"
                "\"\"]";

        QTest::newRow("ops11")
            <<  "t = foo() +\n"
                "    bar()\n"
                "\"\"]"
            <<  std::make_pair(1, 9)
            <<  ""
            <<  "t = foo() +\n"
                "    bar()\n"
                "foobar\n"
                "\"\"]";

        QTest::newRow("ops12")
            <<  "t = foo() + # Comment\n"
                "    bar()\n"
                "\"\"]"
            <<  std::make_pair(1, 9)
            <<  ""
            <<  "t = foo() + # Comment\n"
                "    bar()\n"
                "foobar\n"
                "\"\"]";

        QTest::newRow("ops13")
            <<  "t = foo() -\n"
                "    bar()\n"
                "\"\"]"
            <<  std::make_pair(1, 9)
            <<  ""
            <<  "t = foo() -\n"
                "    bar()\n"
                "foobar\n"
                "\"\"]";

        QTest::newRow("ops14")
            <<  "t = foo() - # Comment\n"
                "    bar()\n"
                "\"\"]"
            <<  std::make_pair(1, 9)
            <<  ""
            <<  "t = foo() - # Comment\n"
                "    bar()\n"
                "foobar\n"
                "\"\"]";

        QTest::newRow("ops15")
            <<  "t = foo() *\n"
                "    bar()\n"
                "\"\"]"
            <<  std::make_pair(1, 9)
            <<  ""
            <<  "t = foo() *\n"
                "    bar()\n"
                "foobar\n"
                "\"\"]";

        QTest::newRow("ops16")
            <<  "t = foo() * # Comment\n"
                "    bar()\n"
                "\"\"]"
            <<  std::make_pair(1, 9)
            <<  ""
            <<  "t = foo() * # Comment\n"
                "    bar()\n"
                "foobar\n"
                "\"\"]";

        QTest::newRow("ops17")
            <<  "t = foo() /\n"
                "    bar()\n"
                "\"\"]"
            <<  std::make_pair(1, 9)
            <<  ""
            <<  "t = foo() /\n"
                "    bar()\n"
                "foobar\n"
                "\"\"]";

        QTest::newRow("ops18")
            <<  "t = foo() / # Comment\n"
                "    bar()\n"
                "\"\"]"
            <<  std::make_pair(1, 9)
            <<  ""
            <<  "t = foo() / # Comment\n"
                "    bar()\n"
                "foobar\n"
                "\"\"]";

        QTest::newRow("ops21")
            <<  "t = foo() +\n"
                "    bar() # Comment\n"
                "\"\"]"
            <<  std::make_pair(1, 19)
            <<  ""
            <<  "t = foo() +\n"
                "    bar() # Comment\n"
                "foobar\n"
                "\"\"]";

        QTest::newRow("ops22")
            <<  "t = foo() + # Comment\n"
                "    bar() # Comment\n"
                "\"\"]"
            <<  std::make_pair(1, 19)
            <<  ""
            <<  "t = foo() + # Comment\n"
                "    bar() # Comment\n"
                "foobar\n"
                "\"\"]";

        QTest::newRow("ops23")
            <<  "t = foo() -\n"
                "    bar() # Comment\n"
                "\"\"]"
            <<  std::make_pair(1, 19)
            <<  ""
            <<  "t = foo() -\n"
                "    bar() # Comment\n"
                "foobar\n"
                "\"\"]";

        QTest::newRow("ops24")
            <<  "t = foo() - # Comment\n"
                "    bar() # Comment\n"
                "\"\"]"
            <<  std::make_pair(1, 19)
            <<  ""
            <<  "t = foo() - # Comment\n"
                "    bar() # Comment\n"
                "foobar\n"
                "\"\"]";

        QTest::newRow("ops25")
            <<  "t = foo() *\n"
                "    bar() # Comment\n"
                "\"\"]"
            <<  std::make_pair(1, 19)
            <<  ""
            <<  "t = foo() *\n"
                "    bar() # Comment\n"
                "foobar\n"
                "\"\"]";

        QTest::newRow("ops26")
            <<  "t = foo() * # Comment\n"
                "    bar() # Comment\n"
                "\"\"]"
            <<  std::make_pair(1, 19)
            <<  ""
            <<  "t = foo() * # Comment\n"
                "    bar() # Comment\n"
                "foobar\n"
                "\"\"]";

        QTest::newRow("ops27")
            <<  "t = foo() /\n"
                "    bar() # Comment\n"
                "\"\"]"
            <<  std::make_pair(1, 19)
            <<  ""
            <<  "t = foo() /\n"
                "    bar() # Comment\n"
                "foobar\n"
                "\"\"]";

        QTest::newRow("ops28")
            <<  "t = foo() / # Comment\n"
                "    bar() # Comment\n"
                "\"\"]"
            <<  std::make_pair(1, 19)
            <<  ""
            <<  "t = foo() / # Comment\n"
                "    bar() # Comment\n"
                "foobar\n"
                "\"\"]";

        QTest::newRow("wordlist01")
            <<  "  for elem in %w[ foo, bar,\n"
                "\"\"]"
            <<  std::make_pair(0, 27)
            <<  ""
            <<  "  for elem in %w[ foo, bar,\n"
                "                  foobar\n"
                "\"\"]";

        QTest::newRow("wordlist02")
            <<  "  for elem in %w[ foo, bar,\n"
                "                  foobar ]\n"
                "\"\"]"
            <<  std::make_pair(1, 26)
            <<  ""
            <<  "  for elem in %w[ foo, bar,\n"
                "                  foobar ]\n"
                "    blah\n"
                "\"\"]";

        QTest::newRow("wordlist11")
            <<  "  for elem in %w< foo, bar,\n"
                "\"\"]"
            <<  std::make_pair(0, 27)
            <<  ""
            <<  "  for elem in %w< foo, bar,\n"
                "                  foobar\n"
                "\"\"]";

        QTest::newRow("wordlist12")
            <<  "  for elem in %w< foo, bar,\n"
                "                  foobar >\n"
                "\"\"]"
            <<  std::make_pair(1, 26)
            <<  ""
            <<  "  for elem in %w< foo, bar,\n"
                "                  foobar >\n"
                "    blah\n"
                "\"\"]";

        QTest::newRow("wordlist21")
            <<  "  for elem in %w| foo, bar,\n"
                "\"\"]"
            <<  std::make_pair(0, 27)
            <<  ""
            <<  "  for elem in %w| foo, bar,\n"
                "                  foobar\n"
                "\"\"]";

        QTest::newRow("wordlist22")
            <<  "  for elem in %w| foo, bar,\n"
                "                  foobar |\n"
                "\"\"]"
            <<  std::make_pair(1, 26)
            <<  ""
            <<  "  for elem in %w| foo, bar,\n"
                "                  foobar |\n"
                "    blah\n"
                "\"\"]";

        QTest::newRow("multiline1")
            <<  "# multiline1.txt"
            <<  std::make_pair(2, 17)
            <<  ""
            <<  "# multiline1.txt";

        QTest::newRow("multiline2")
            <<  "# multiline2.txt"
            <<  std::make_pair(2, 17)
            <<  ""
            <<  "# multiline2.txt";

        QTest::newRow("multiline3")
            <<  "# multiline3.txt"
            <<  std::make_pair(1, 15)
            <<  ""
            <<  "# multiline3.txt";

        QTest::newRow("multiline4")
            <<  "# multiline4.txt"
            <<  std::make_pair(2, 19)
            <<  ""
            <<  "# multiline4.txt";

        QTest::newRow("multiline5")
            <<  "# multiline5.txt"
            <<  std::make_pair(2, 19)
            <<  ""
            <<  "# multiline5.txt";

        QTest::newRow("multiline6")
            <<  "# multiline6.txt"
            <<  std::make_pair(1, 13)
            <<  ""
            <<  "# multiline6.txt";

        QTest::newRow("multiline7")
            <<  ""
            <<  std::make_pair(0, 13)
            <<  ""
            <<  "";

        QTest::newRow("multiline8")
            <<  "def foo(array)\n"
                "  array.each_with_index \\\n"
                "      do\n"
                "\"\"]"
            <<  std::make_pair(2, 8)
            <<  ""
            <<  "def foo(array)\n"
                "  array.each_with_index \\\n"
                "      do\n"
                "    bar\n"
                "  end\n"
                "\"\"]";

        QTest::newRow("multiline9")
            <<  "if test \\"
            <<  std::make_pair(2, 8)
            <<  ""
            <<  "if test \\";

        QTest::newRow("multiline10")
            <<  "if test1 &&\n"
                "# still part of condition\n"
                "   test2\n"
                "\"\"]"
            <<  std::make_pair(2, 8)
            <<  ""
            <<  "if test1 &&\n"
                "# still part of condition\n"
                "   test2\n"
                "  foo\n"
                "\"\"]";

        QTest::newRow("plist1")
            <<  "  foobar(foo,\n"
                "\"\"]"
            <<  std::make_pair(0, 13)
            <<  ""
            <<  "  foobar(foo,\n"
                "         bar\n"
                "\"\"]";

        QTest::newRow("plist2")
            <<  "  foobar(foo, foo,\n"
                "\"\"]"
            <<  std::make_pair(0, 18)
            <<  ""
            <<  "  foobar(foo, foo,\n"
                "         bar\n"
                "\"\"]";

        QTest::newRow("plist3")
            <<  "  foobar(foo,\n"
                "         bar)\n"
                "\"\"]"
            <<  std::make_pair(1, 13)
            <<  ""
            <<  "  foobar(foo,\n"
                "         bar)\n"
                "  blah\n"
                "\"\"]";

        QTest::newRow("plist4")
            <<  "  foobar(foo,\n"
                "         bar,\n"
                "\"\"]"
            <<  std::make_pair(1, 13)
            <<  ""
            <<  "  foobar(foo,\n"
                "         bar,\n"
                "         baz\n"
                "\"\"]";

        QTest::newRow("plist5")
            <<  "  foobar(foo,\n"
                "         bar, bar,\n"
                "\"\"]"
            <<  std::make_pair(1, 18)
            <<  ""
            <<  "  foobar(foo,\n"
                "         bar, bar,\n"
                "         baz\n"
                "\"\"]";

        QTest::newRow("plist6")
            <<  "  foobar(foo,\n"
                "         bar,\n"
                "         baz)\n"
                "\"\"]"
            <<  std::make_pair(2, 13)
            <<  ""
            <<  "  foobar(foo,\n"
                "         bar,\n"
                "         baz)\n"
                "  blah\n"
                "\"\"]";

        QTest::newRow("plist7")
            <<  "  foobar(foo(bar,\n"
                "\"\"]"
            <<  std::make_pair(0, 17)
            <<  ""
            <<  "  foobar(foo(bar,\n"
                "             baz\n"
                "\"\"]";

        QTest::newRow("plist8")
            <<  "  foobar(foo(bar,\n"
                "             baz),\n"
                "\"\"]"
            <<  std::make_pair(1, 18)
            <<  ""
            <<  "  foobar(foo(bar,\n"
                "             baz),\n"
                "         blah\n"
                "\"\"]";

        QTest::newRow("plist9")
            <<  "  foobar(foo(bar,\n"
                "             baz),\n"
                "         foobaz(),\n"
                "\"\"]"
            <<  std::make_pair(2, 18)
            <<  ""
            <<  "  foobar(foo(bar,\n"
                "             baz),\n"
                "         foobaz(),\n"
                "         blah\n"
                "\"\"]";

        QTest::newRow("plist10")
            <<  "  foobar(foo(bar,\n"
                "             baz),\n"
                "         blah)\n"
                "\"\"]"
            <<  std::make_pair(2, 14)
            <<  ""
            <<  "  foobar(foo(bar,\n"
                "             baz),\n"
                "         blah)\n"
                "  foobaz\n"
                "\"\"]";

        QTest::newRow("plist11")
            <<  "  foobar( foo,\n"
                "\"\"]"
            <<  std::make_pair(0, 14)
            <<  ""
            <<  "  foobar( foo,\n"
                "          bar\n"
                "\"\"]";

        QTest::newRow("plist12")
            <<  "  foobar( foo, foo,\n"
                "\"\"]"
            <<  std::make_pair(0, 19)
            <<  ""
            <<  "  foobar( foo, foo,\n"
                "          bar\n"
                "\"\"]";

        QTest::newRow("plist13")
            <<  "  foobar( foo,\n"
                "          bar )\n"
                "\"\"]"
            <<  std::make_pair(1, 15)
            <<  ""
            <<  "  foobar( foo,\n"
                "          bar )\n"
                "  blah\n"
                "\"\"]";

        QTest::newRow("plist14")
            <<  "  foobar ( foo,\n"
                "           bar,\n"
                "\"\"]"
            <<  std::make_pair(1, 15)
            <<  ""
            <<  "  foobar ( foo,\n"
                "           bar,\n"
                "           baz\n"
                "\"\"]";

        QTest::newRow("plist15")
            <<  "  foobar ( foo,\n"
                "           bar, bar,\n"
                "\"\"]"
            <<  std::make_pair(1, 20)
            <<  ""
            <<  "  foobar ( foo,\n"
                "           bar, bar,\n"
                "           baz\n"
                "\"\"]";

        QTest::newRow("plist16")
            <<  "  foobar ( foo,\n"
                "           bar,\n"
                "           baz )\n"
                "\"\"]"
            <<  std::make_pair(2, 16)
            <<  ""
            <<  "  foobar ( foo,\n"
                "           bar,\n"
                "           baz )\n"
                "  blah\n"
                "\"\"]";

        QTest::newRow("plist17")
            <<  "  foobar ( foo ( bar,\n"
                "\"\"]"
            <<  std::make_pair(0, 21)
            <<  ""
            <<  "  foobar ( foo ( bar,\n"
                "                 baz\n"
                "\"\"]";

        QTest::newRow("plist18")
            <<  "  foobar ( foo ( bar,\n"
                "                 baz ),\n"
                "\"\"]"
            <<  std::make_pair(1, 23)
            <<  ""
            <<  "  foobar ( foo ( bar,\n"
                "                 baz ),\n"
                "           blah\n"
                "\"\"]";

        QTest::newRow("plist19")
            <<  "  foobar ( foo ( bar,\n"
                "                 baz ),\n"
                "           foobaz(),\n"
                "\"\"]"
            <<  std::make_pair(2, 20)
            <<  ""
            <<  "  foobar ( foo ( bar,\n"
                "                 baz ),\n"
                "           foobaz(),\n"
                "           blah\n"
                "\"\"]";

        QTest::newRow("plist20")
            <<  "  foobar ( foo ( bar,\n"
                "                 baz ),\n"
                "           blah )\n"
                "\"\"]"
            <<  std::make_pair(2, 17)
            <<  ""
            <<  "  foobar ( foo ( bar,\n"
                "                 baz ),\n"
                "           blah )\n"
                "  foobaz\n"
                "\"\"]";

        QTest::newRow("plist_comment1")
            <<  "  foobar(foo, # comment\n"
                "\"\"]"
            <<  std::make_pair(0, 23)
            <<  ""
            <<  "  foobar(foo, # comment\n"
                "         bar\n"
                "\"\"]";

        QTest::newRow("plist_comment2")
            <<  "  foobar(foo, foo, # comment\n"
                "\"\"]"
            <<  std::make_pair(0, 28)
            <<  ""
            <<  "  foobar(foo, foo, # comment\n"
                "         bar\n"
                "\"\"]";

        QTest::newRow("plist_comment3")
            <<  "  foobar(foo,\n"
                "         # comment\n"
                "         bar) # comment\n"
                "\"\"]"
            <<  std::make_pair(2, 23)
            <<  ""
            <<  "  foobar(foo,\n"
                "         # comment\n"
                "         bar) # comment\n"
                "  blah\n"
                "\"\"]";

        QTest::newRow("plist_comment4")
            <<  "  foobar(foo,\n"
                "         bar, # comment\n"
                "         # comment\n"
                "\"\"]"
            <<  std::make_pair(2, 18)
            <<  ""
            <<  "  foobar(foo,\n"
                "         bar, # comment\n"
                "         # comment\n"
                "         baz\n"
                "\"\"]";

        QTest::newRow("plist_comment5")
            <<  "  foobar(foo,\n"
                "         bar, bar, # comment\n"
                "                   # comment\n"
                "\"\"]"
            <<  std::make_pair(2, 28)
            <<  ""
            <<  "  foobar(foo,\n"
                "         bar, bar, # comment\n"
                "                   # comment\n"
                "         baz\n"
                "\"\"]";

        QTest::newRow("plist_comment6")
            <<  "  foobar(foo,\n"
                "         bar,\n"
                "         baz) # comment,\n"
                "\"\"]"
            <<  std::make_pair(2, 24)
            <<  ""
            <<  "  foobar(foo,\n"
                "         bar,\n"
                "         baz) # comment,\n"
                "  blah\n"
                "\"\"]";

        QTest::newRow("plist_comment7")
            <<  "  foobar(foo(bar, # comment\n"
                "                  # comment\n"
                "\"\"]"
            <<  std::make_pair(1, 27)
            <<  ""
            <<  "  foobar(foo(bar, # comment\n"
                "                  # comment\n"
                "             baz\n"
                "\"\"]";

        QTest::newRow("plist_comment8")
            <<  "  foobar(foo(bar,\n"
                "  # comment\n"
                "             baz), # comment\n"
                "\"\"]"
            <<  std::make_pair(2, 28)
            <<  ""
            <<  "  foobar(foo(bar,\n"
                "  # comment\n"
                "             baz), # comment\n"
                "         blah\n"
                "\"\"]";

        QTest::newRow("plist_comment9")
            <<  "  foobar(foo(bar,\n"
                "             baz), # comment\n"
                "         foobaz(), # comment\n"
                "                   # comment\n"
                "\"\"]"
            <<  std::make_pair(3, 28)
            <<  ""
            <<  "  foobar(foo(bar,\n"
                "             baz), # comment\n"
                "         foobaz(), # comment\n"
                "                   # comment\n"
                "         blah\n"
                "\"\"]";

        QTest::newRow("plist_comment10")
            <<  "  foobar(foo(bar,\n"
                "             baz),\n"
                "       # comment\n"
                "         blah)\n"
                "       # comment\n"
                "\"\"]"
            <<  std::make_pair(4, 16)
            <<  ""
            <<  "  foobar(foo(bar,\n"
                "             baz),\n"
                "       # comment\n"
                "         blah)\n"
                "       # comment\n"
                "  foobaz\n"
                "\"\"]";

        QTest::newRow("comment1")
            <<  "# comment1.txt\n"
                "if foo\n"
                "  # Comment\n"
                "\"\"]"
            <<  std::make_pair(2, 11)
            <<  ""
            <<  "# comment1.txt\n"
                "if foo\n"
                "  # Comment\n"
                "end\n"
                "\"\"]";

        QTest::newRow("comment2")
            <<  "# comment2.txt\n"
                "if foo\n"
                "  # Comment\n"
                "\"\"]"
            <<  std::make_pair(2, 11)
            <<  ""
            <<  "# comment2.txt\n"
                "if foo\n"
                "  # Comment\n"
                "  bar\n"
                "end\n"
                "\"\"]";

        QTest::newRow("comment3")
            <<  "# comment3.txt\n"
                "if foo\n"
                "  # Comment\n"
                "\"\"]"
            <<  std::make_pair(2, 11)
            <<  ""
            <<  "# comment3.txt\n"
                "if foo\n"
                "  # Comment\n"
                "  bar\n"
                "  # Another comment\n"
                "end\n"
                "\"\"]";

        QTest::newRow("comment4")
            <<  "# comment4.txt\n"
                "if foo\n"
                "  # Comment\n"
                "\"\"]"
            <<  std::make_pair(2, 11)
            <<  ""
            <<  "# comment4.txt\n"
                "if foo\n"
                "  # Comment\n"
                "  bar # Another comment\n"
                "end\n"
                "\"\"]";

        QTest::newRow("comment5")
            <<  "# comment5.txt\n"
                "if foo\n"
                "     # Misplaced comment\n"
                "\"\"]"
            <<  std::make_pair(2, 24)
            <<  ""
            <<  "# comment5.txt\n"
                "if foo\n"
                "     # Misplaced comment\n"
                "  bar\n"
                "end\n"
                "\"\"]";

        QTest::newRow("comment6")
            <<  "# comment6.txt\n"
                "if foo\n"
                "# Misplaced comment\n"
                "\"\"]"
            <<  std::make_pair(2, 19)
            <<  ""
            <<  "# comment6.txt\n"
                "if foo\n"
                "# Misplaced comment\n"
                "  bar\n"
                "end\n"
                "\"\"]";

        QTest::newRow("heredoc1")
            <<  "doc = <<EOF\n"
                "\"\"]"
            <<  std::make_pair(0, 11)
            <<  ""
            <<  "doc = <<EOF\n"
                "koko\n"
                "\"\"]";

        QTest::newRow("heredoc2")
            <<  "doc = <<EOF\n"
                "\"\"]"
            <<  std::make_pair(0, 11)
            <<  ""
            <<  "doc = <<EOF\n"
                "\n"
                "koko\n"
                "\"\"]";

        QTest::newRow("heredoc3")
            <<  "doc = <<EOF\n"
                "if foo\n"
                "\"\"]"
            <<  std::make_pair(1, 6)
            <<  ""
            <<  "doc = <<EOF\n"
                "if foo\n"
                "koko\n"
                "\"\"]";

        QTest::newRow("heredoc4")
            <<  "doc = <<EOF\n"
                "if foo\n"
                "\"\"]"
            <<  std::make_pair(1, 6)
            <<  ""
            <<  "doc = <<EOF\n"
                "if foo\n"
                "\n"
                "koko\n"
                "\"\"]";

        QTest::newRow("heredoc5")
            <<  "if foo\n"
                "doc = <<EOF\n"
                "\"\"]"
            <<  std::make_pair(1, 11)
            <<  ""
            <<  "if foo\n"
                "doc = <<EOF\n"
                "koko\n"
                "\"\"]";

        QTest::newRow("heredoc6")
            <<  "if foo\n"
                "doc = <<EOF\n"
                "\"\"]"
            <<  std::make_pair(1, 11)
            <<  ""
            <<  "if foo\n"
                "doc = <<EOF\n"
                "\n"
                "koko\n"
                "\"\"]";

        QTest::newRow("block_comment1")
            <<  "=begin\n"
                "\"\"]"
            <<  std::make_pair(0, 6)
            <<  ""
            <<  "=begin\n"
                "koko\n"
                "\"\"]";

        QTest::newRow("block_comment2")
            <<  "=begin\n"
                "\"\"]"
            <<  std::make_pair(0, 6)
            <<  ""
            <<  "=begin\n"
                "\n"
                "koko\n"
                "\"\"]";

        QTest::newRow("block_comment3")
            <<  "=begin\n"
                "if foo\n"
                "\"\"]"
            <<  std::make_pair(1, 6)
            <<  ""
            <<  "=begin\n"
                "if foo\n"
                "koko\n"
                "\"\"]";

        QTest::newRow("block_comment4")
            <<  "=begin\n"
                "if foo\n"
                "\"\"]"
            <<  std::make_pair(1, 6)
            <<  ""
            <<  "=begin\n"
                "if foo\n"
                "\n"
                "koko\n"
                "\"\"]";

        QTest::newRow("block_comment5")
            <<  "if foo\n"
                "=begin\n"
                "\"\"]"
            <<  std::make_pair(1, 6)
            <<  ""
            <<  "if foo\n"
                "=begin\n"
                "koko\n"
                "\"\"]";

        QTest::newRow("block_comment6")
            <<  "if foo\n"
                "=begin\n"
                "\"\"]"
            <<  std::make_pair(1, 6)
            <<  ""
            <<  "if foo\n"
                "=begin\n"
                "\n"
                "koko\n"
                "\"\"]";

        QTest::newRow("block_comment7")
            <<  "if foo\n"
                "=begin\n"
                "=end\n"
                "\"\"]"
            <<  std::make_pair(2, 4)
            <<  ""
            <<  "if foo\n"
                "=begin\n"
                "=end\n"
                "  koko\n"
                "\"\"]";

        QTest::newRow("block_comment8")
            <<  "if foo\n"
                "=begin\n"
                "=end\n"
                "\"\"]"
            <<  std::make_pair(2, 4)
            <<  ""
            <<  "if foo\n"
                "=begin\n"
                "=end\n"
                "\n"
                "  koko\n"
                "\"\"]";
    }
};


QTEST_MAIN(Test)
#include "test_indenter_ruby.moc"
