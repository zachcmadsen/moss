cmake_minimum_required(VERSION 3.27)

project(moss)

set(CMAKE_CXX_STANDARD 23)

set(SOURCES main.cpp cpu.cpp)

add_executable(moss ${SOURCES})
target_include_directories(moss PRIVATE ${CMAKE_SOURCE_DIR})

add_custom_target(tidy
    clang-tidy -p ${CMAKE_BINARY_DIR}/compile_commands.json -header-filter=cpu.h -checks=bugprone-*,clang-analyzer-*,cppcoreguidelines-*,misc-*,modernize-*,performance-*,readability-*,-cppcoreguidelines-avoid-magic-numbers,-misc-non-private-member-variables-in-classes,-modernize-use-trailing-return-type,-readability-identifier-length,-readability-magic-numbers ${SOURCES}
    DEPENDS ${SOURCES}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
