.. Qutepart documentation master file, created by
   sphinx-quickstart on Thu May 21 22:14:20 2020.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.


Qutepart's Documentation
========================

Qutepart is a code editor widget for C++/Qt5. It doesn't have any external dependencies.

Qutepart highlightes your code and implements many advanced editor features.

The project has a sibling implemented in Python. See `Python Qutepart <https://github.com/andreikop/qutepart>`_

.. contents::

Features
--------

All features are configurable

* Syntax highlighting for 196 languages
* Smart indentation algorithms:
    * Generic
    * Python
    * Ruby
    * XML
    * Lisp
    * Scheme
    * C, C++
    * JavaScript
    * Java
    * PHP
    * Go
    * And many others
* Line numbers
* Bracket highlighting
* Visible witespaces
* Marker for too long lines
* Bookmarks
* Current line highlighting
* Advanced edit operations:
    - Delete selection or line
    - Duplicate selection or line
    - Move selected lines up or down
    - Copy-paste current or selected lines
    - Join lines

Basic usage
-----------

.. code-block:: cpp

    #include "qutepart.h"

    Qutepart::Qutepart qutepart;

    Qutepart::LangInfo langInfo = Qutepart::chooseLanguage(
        QString::null, QString::null, filePath);
    if (langInfo.isValid()) {
        qutepart.setHighlighter(langInfo.id);
        qutepart.setIndentAlgorithm(langInfo.indentAlg);
    }

    qutepart.setPlainText("int foo(int bar)\n{}");

    qutepart.show()

Detecting programming language
------------------------------
The function ``Qutepart::chooseLanguage( )`` detects syntax using next parameters. Later ``LangInfo`` can be used to apply syntax highlighter and indenter.


.. doxygenfunction:: Qutepart::chooseLanguage

.. doxygenstruct:: Qutepart::LangInfo
    :members:

Smart indentation
-----------------
Qutepart supports smart indentation algorithms for many languages.

.. doxygenenum:: Qutepart::IndentAlg

Accessing document lines
------------------------
Text of the edited document is accessible with method ``toPlainText()``, but the method is quite heavy-weight, do not use it unless you need whole file contents.

Individual lines can be accessed using ``QTextDocument``, ``QTextCursor``, ``QTextBlock`` files, but this API is quite low-level. Qutepart provides simpler API which can be used to do many basic operations with the text.

Printing lines one by one:

.. code-block:: cpp

    Qutepart::Qutepart qutepart;
    qutepart.setPlainText(fileContents);
    Qutepart::Lines lines;

    for (Qutepart::Line line: lines) {
        qDebug() << line.text();
    }

Inserting new line before the last line

.. code-block:: cpp

    int lastLineIndex = lines.count() - 1;
    lines.insertAt(lastLineIndex - 1, "New line text");


.. doxygenclass:: Qutepart::Lines
   :members:

.. doxygenclass:: Qutepart::Line
   :members:

.. doxygenclass:: Qutepart::LineIterator
   :members:

Cursor position
---------------

.. doxygenstruct:: Qutepart::TextCursorPosition
   :members:


Atomic operations
-----------------
It is often necessary to make multiple changes in a file which can be undo/redo as a single operation. Use helper class ``AtomicEditOperation``.


.. doxygenclass:: Qutepart::AtomicEditOperation
   :members:

Whole Qutepart class API
------------------------

The widget is based on ``QPlainTextEdit``. Read parent class documentation for general understanding how it works.
Quteparts own methods provide additional features and convenience APIs.

.. doxygenclass:: Qutepart::Qutepart
   :members:
   :undoc-members:

Using only the syntax highlighter
---------------------------------
In some cases it might be useful to use only syntax highligher but not other Qutepart functionality. It is possible to create a ``QSyntaxHighlighter`` subclass instance and apply it to ``QPlainTextEdit``.


.. code-block:: cpp

    #include "qutepart.h"
    #include "hl_factory.h"

    Qutepart::LangInfo langInfo =
        Qutepart::chooseLanguage(QString::null, QString::null, filePath);

    QPlainTextEdit textEdit;
    QSyntaxHighlighter* highlighter =
        Qutepart::makeHighlighter(langInfo.id, textEdit.document());


.. doxygenfile:: hl_factory.h

Indices and tables
==================

* :ref:`genindex`
* :ref:`modindex`
* :ref:`search`
