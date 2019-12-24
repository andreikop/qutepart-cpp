cmake_minimum_required(VERSION 3.1.0)

project(qutepart)

# Instruct CMake to run moc automatically when needed
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 14)

# Find the QtWidgets library
find_package(Qt5Core CONFIG REQUIRED)
find_package(Qt5Widgets CONFIG REQUIRED)
find_package(Qt5Test CONFIG REQUIRED)

# Populate a CMake variable with the sources
set(qutepart_SRCS
    qutepart-syntax-files.qrc
    src/qutepart.cpp
    src/text_pos.cpp
    src/char_iterator.cpp
    src/text_block_utils.cpp
    src/bracket_highlighter.cpp
    src/side_areas.cpp
    src/hl_factory.cpp
    src/hl/context.cpp
    src/hl/language.cpp
    src/hl/loader.cpp
    src/hl/rules.cpp
    src/hl/syntax_highlighter.cpp
    src/hl/style.cpp
    src/hl/context_stack.cpp
    src/hl/context_switcher.cpp
    src/hl/text_block_user_data.cpp
    src/hl/text_to_match.cpp
    src/hl/match_result.cpp
    src/hl/language_db_generated.cpp
    src/hl/language_db.cpp
    src/hl/text_type.cpp
    src/indent/indenter.cpp
    src/indent/indent_funcs.cpp
    src/indent/alg_impl.cpp
    src/indent/alg_lisp.cpp
    src/indent/alg_xml.cpp
    src/indent/alg_scheme.cpp
    src/indent/alg_python.cpp
    src/indent/alg_ruby.cpp
    src/indent/alg_cstyle.cpp
    )

include_directories(include)
include_directories(src)
include_directories(test)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Tell CMake to create the qutepart library
add_library(qutepart ${qutepart_SRCS} include/qutepart.h)

# Use the Widgets module from Qt 5
target_link_libraries(qutepart Qt5::Core Qt5::Widgets)

# Build example executable
add_executable(editor editor.cpp)
target_link_libraries(editor Qt5::Core Qt5::Widgets qutepart)

# Install only library, not binaries
install(TARGETS qutepart DESTINATION lib)
install(FILES include/hl_factory.h include/qutepart.h DESTINATION include/qutepart)

# Tests
enable_testing()

function(indent_test name)
    add_executable(test-indenter-${name} test/base_test.cpp test/test_indenter_${name}.cpp)
    target_link_libraries(test-indenter-${name} Qt5::Test Qt5::Core Qt5::Widgets qutepart)
    add_test(NAME test-indenter-${name} COMMAND test-indenter-${name})
endfunction()

indent_test(normal)
indent_test(python)
indent_test(ruby)
indent_test(cstyle)
indent_test(xml)
indent_test(scheme)
indent_test(lisp)
indent_test(haskel)
