include(FetchContent)

message(STATUS "Fetching doctest...")
FetchContent_Declare(doctest GIT_REPOSITORY https://github.com/doctest/doctest GIT_TAG v2.4.11)

message(STATUS "Fetching simdjson...")
FetchContent_Declare(
  simdjson
  GIT_REPOSITORY https://github.com/simdjson/simdjson
  GIT_TAG v3.8.0
  GIT_SHALLOW TRUE
  SYSTEM)

FetchContent_MakeAvailable(doctest simdjson)

add_executable(moss_test main.cpp functional.cpp processor.cpp)
target_link_libraries(moss_test moss doctest::doctest simdjson::simdjson)

include(${PROJECT_SOURCE_DIR}/cmake/moss.cmake)
moss_compile_options(moss_test)

include(${doctest_SOURCE_DIR}/scripts/cmake/doctest.cmake)
doctest_discover_tests(moss_test)
