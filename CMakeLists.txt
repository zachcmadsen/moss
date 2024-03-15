cmake_minimum_required(VERSION 3.27)

project(moss)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Convert MOSS_CXX_FLAGS to a list to work with target_compile_options.
separate_arguments(MOSS_CXX_FLAGS_LIST NATIVE_COMMAND ${MOSS_CXX_FLAGS})

add_subdirectory(src)

# Don't include test targets if we're in a subdirectory, i.e., if the libary
# is used in another project.
if (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    add_subdirectory(tests)
endif()
