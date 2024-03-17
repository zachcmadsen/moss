function(moss_compile_options TARGET)
    if (MOSS_CXX_FLAGS)
        # Convert MOSS_CXX_FLAGS to a list to work with target_compile_options.
        separate_arguments(MOSS_CXX_FLAGS_LIST NATIVE_COMMAND ${MOSS_CXX_FLAGS})
        target_compile_options(${TARGET} PRIVATE ${MOSS_CXX_FLAGS_LIST})
    endif()
endfunction()
