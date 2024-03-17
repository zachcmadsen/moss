cmake_minimum_required(VERSION 3.27)

project(moss)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(src)

# Only include tests if moss isn't used as a library, i.e., if it's not in a
# subdirectory.
# TODO: The executable should be excluded too.
if (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    enable_testing()
    add_subdirectory(tests)
endif()
