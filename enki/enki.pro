TARGET = enki-ng

CONFIG += debug
CONFIG += c++11

CONFIG += qt
QT += widgets

INCLUDEPATH += ../include

SOURCES = \
    ../src/qutepart.cpp \
    ../src/hl_factory.cpp \
    ../src/hl/context.cpp \
    ../src/hl/language.cpp \
    ../src/hl/loader.cpp \
    ../src/hl/rules.cpp \
    ../src/hl/syntax_highlighter.cpp \
    ../src/hl/style.cpp \
    ../src/hl/context_stack.cpp \
    ../src/hl/context_switcher.cpp \
    ../src/hl/text_block_user_data.cpp \
    ../src/hl/text_to_match.cpp \
    ../src/hl/match_result.cpp \
    ../src/hl/language_db_generated.cpp \
    ../src/hl/language_db.cpp \
    \
    src/main.cpp \
    src/main_window.cpp


HEADERS = \
    ../include/qutepart.h \
    ../include/hl_factory.h \
    ../src/hl/context.h \
    ../src/hl/language.h \
    ../src/hl/rules.h \
    ../src/hl/syntax_highlighter.h \
    ../src/hl/style.h \
    ../src/hl/context_stack.h \
    ../src/hl/context_switcher.h \
    ../src/hl/text_to_match.h \
    ../src/hl/match_result.h \
    ../src/hl/language_db.h \
    \
    src/main_window.h


RESOURCES = ../syntax-files.qrc
