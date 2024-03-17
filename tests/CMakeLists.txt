find_package(doctest CONFIG REQUIRED)
find_package(simdjson CONFIG REQUIRED)

add_executable(moss_test main.cpp functional.cpp processor.cpp)
target_link_libraries(moss_test moss doctest::doctest simdjson::simdjson)

include(${PROJECT_SOURCE_DIR}/cmake/moss.cmake)
moss_compile_options(moss_test)

include(doctest)
doctest_discover_tests(moss_test)
