cmake_minimum_required(VERSION 3.1.0)

project(qutepart)

# Instruct CMake to run moc automatically when needed
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 14)

# Find the QtWidgets library
find_package(Qt5Core CONFIG REQUIRED)
find_package(Qt5Widgets CONFIG REQUIRED)

# Populate a CMake variable with the sources
set(qutepart_SRCS
    qutepart-syntax-files.qrc
    src/qutepart.cpp
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
    src/indent/indenter.cpp
    )

include_directories(include)
include_directories(src)

# Tell CMake to create the qutepart library
add_library(qutepart ${qutepart_SRCS})

# Use the Widgets module from Qt 5
target_link_libraries(qutepart Qt5::Core Qt5::Widgets)

# Build example executable
add_executable(editor editor.cpp include/qutepart.h)
target_link_libraries(editor Qt5::Core Qt5::Widgets qutepart)

# Install only library, not binaries
install(TARGETS qutepart DESTINATION lib)
install(FILES include/hl_factory.h include/qutepart.h DESTINATION include/qutepart)
