TARGET = editor

CONFIG += debug
CONFIG += c++11

CONFIG += qt
QT += widgets

SOURCES = \
    src/context.cpp \
    src/language.cpp \
    src/loader.cpp \
    src/main.cpp \
    src/qutepart.cpp \
    src/syntax_highlighter.cpp

HEADERS = \
    src/context.h \
    src/language.h \
    src/qutepart.h \
    src/syntax_highlighter.h
