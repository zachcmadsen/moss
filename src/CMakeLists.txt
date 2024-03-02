add_library(cpu STATIC cpu.cpp)
target_include_directories(cpu PUBLIC ${CMAKE_SOURCE_DIR}/include)

add_executable(moss main.cpp)
target_link_libraries(moss cpu)
