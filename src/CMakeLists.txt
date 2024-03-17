include(${PROJECT_SOURCE_DIR}/cmake/moss.cmake)

add_library(moss STATIC cpu.cpp)
target_include_directories(moss PUBLIC ${CMAKE_SOURCE_DIR}/include)
moss_compile_options(moss)

add_executable(moss_exe main.cpp)
target_link_libraries(moss_exe moss)
moss_compile_options(moss_exe)
# Rename the executable to moss (the executable target can't have the same name
# as the library target).
# TODO: Might need RUNTIME_OUTPUT_NAME for MSVC.
set_target_properties(moss_exe PROPERTIES OUTPUT_NAME moss)
