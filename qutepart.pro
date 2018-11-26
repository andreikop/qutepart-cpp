TARGET = editor

CONFIG += debug

CONFIG += qt
QT += widgets

SOURCES = \
    src/main.cpp \
    src/qutepart.cpp \
    src/syntax_highlighter.cpp \
    src/language.cpp \
    src/loader.cpp

HEADERS = \
    src/qutepart.h \
    src/syntax_highlighter.h \
    src/language.h \
    src/language.cpp
