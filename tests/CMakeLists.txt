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

enable_testing()

add_executable(moss_test cpu_test.cpp)
target_link_libraries(moss_test moss GTest::gtest_main simdjson)
if (MOSS_CXX_FLAGS)
  target_compile_options(moss_test PRIVATE ${MOSS_CXX_FLAGS_LIST})
endif()

include(GoogleTest)
gtest_discover_tests(moss_test)
