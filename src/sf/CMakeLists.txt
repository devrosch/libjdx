set(HEADER_LIST
    # none
)

add_library(sf
    Wrapper.cpp
    ${HEADER_LIST}
)

# All users of this library will need at least C++17
target_compile_features(sf PUBLIC cxx_std_17)

target_link_libraries(sf PUBLIC api jdx)

if (EMSCRIPTEN)
    # make sure that code not used in this target is retained, see: https://emscripten.org/docs/porting/connecting_cpp_and_javascript/embind.html
    # for making this work in CMake, see: https://stackoverflow.com/a/59994837
    # alternative discussion https://stackoverflow.com/questions/53071878/using-whole-archive-linker-option-with-cmake-and-libraries-with-other-library
    target_link_options(api INTERFACE "LINKER:--whole-archive")
endif()
