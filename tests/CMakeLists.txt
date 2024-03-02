include(FetchContent)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/e4fdb87e76b9fc4b01c54ad81aea19d6e994b994.zip
)
FetchContent_MakeAvailable(googletest)

FetchContent_Declare(
  simdjson
  GIT_REPOSITORY https://github.com/simdjson/simdjson.git
  GIT_TAG  tags/v3.6.0
  GIT_SHALLOW TRUE)
FetchContent_MakeAvailable(simdjson)
# Ignore compiler warnings from simdjson.
target_compile_options(simdjson PRIVATE -w)

enable_testing()

add_executable(cpu_test functional.cpp processor_tests.cpp)
target_link_libraries(cpu_test cpu GTest::gtest_main simdjson)

include(GoogleTest)
gtest_discover_tests(cpu_test)
