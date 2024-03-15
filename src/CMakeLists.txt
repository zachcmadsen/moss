add_library(moss STATIC cpu.cpp)
target_include_directories(moss PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_compile_options(moss PRIVATE ${MOSS_CXX_FLAGS_LIST})

add_executable(main main.cpp)
target_link_libraries(main moss)
target_compile_options(main PRIVATE ${MOSS_CXX_FLAGS_LIST})
