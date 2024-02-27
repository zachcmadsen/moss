cmake_minimum_required(VERSION 3.27)

project(moss)

set(CMAKE_CXX_STANDARD 23)

set(SOURCES main.cpp cpu.cpp)

add_library(doctest INTERFACE)
target_include_directories(doctest SYSTEM INTERFACE external/doctest/doctest)

add_library(simdjson STATIC
    external/simdjson/singleheader/simdjson.cpp
    external/simdjson/singleheader/simdjson.h)
target_include_directories(simdjson SYSTEM INTERFACE external/simdjson/singleheader)
# Ignore compiler warnings from simdjson.
target_compile_options(simdjson PRIVATE -w)

add_executable(moss ${SOURCES})
target_include_directories(moss PRIVATE ${CMAKE_SOURCE_DIR})
target_link_libraries(moss PRIVATE doctest)
target_link_libraries(moss PRIVATE simdjson)

add_custom_target(tidy
    clang-tidy -p ${CMAKE_BINARY_DIR}/compile_commands.json -header-filter=cpu.h -checks=bugprone-*,clang-analyzer-*,cppcoreguidelines-*,misc-*,modernize-*,performance-*,readability-*,-cppcoreguidelines-avoid-magic-numbers,-misc-non-private-member-variables-in-classes,-modernize-use-trailing-return-type,-readability-identifier-length,-readability-magic-numbers ${SOURCES}
    DEPENDS ${SOURCES}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
