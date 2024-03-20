cmake_minimum_required(VERSION 3.27)

function(moss_add_example TARGET)
    add_executable(${TARGET} ${TARGET}.c)
    # The common target's libraries don't include functionality that isn't
    # absolutely necessary. We need to explicitly link to libraries we want.
    target_link_libraries(${TARGET} exit-loop init-stack)
    # llvm-mos-sdk picks the MinSizeRel build type by default, but it doesn't
    # seem to add -Os. 
    target_compile_options(${TARGET} PRIVATE -Os)
endfunction()

# TODO: Select the right download link for Windows or Linux.
include(FetchContent)
FetchContent_Declare(
    llvm-mos-sdk
    URL https://github.com/llvm-mos/llvm-mos-sdk/releases/latest/download/llvm-mos-windows.7z)
FetchContent_MakeAvailable(llvm-mos-sdk)

set(CMAKE_PREFIX_PATH ${llvm-mos-sdk_SOURCE_DIR})
# Target STATIC_LIBRARY type avoids running the linker when cmake tests the C
# compiler, which wouldn't work because it needs a custom linker script.
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

set(LLVM_MOS_PLATFORM common)
find_package(llvm-mos-sdk REQUIRED)

project(examples)

# Every example needs the linker script.
configure_file(${CMAKE_SOURCE_DIR}/link.ld link.ld COPYONLY)

moss_add_example(fib)
