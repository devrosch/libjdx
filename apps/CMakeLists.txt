# Tests need to be added as executables first
add_executable(sf_main
    Main.cpp
)

# I'm using C++17 in the test
target_compile_features(sf_main PRIVATE cxx_std_17)

# Should be linked to the library
target_link_libraries(sf_main PRIVATE sf)

if (EMSCRIPTEN)
    # Emscripten
    # Avoid "undefined symbol" errors, see: https://stackoverflow.com/a/37869647
    target_link_options(sf_main PRIVATE "--bind")
endif()

