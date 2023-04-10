# Tests need to be added as executables first
add_executable(libsf
    Main.cpp
)

# Should be linked to the library
target_link_libraries(libsf PUBLIC sf)

if (EMSCRIPTEN)
    # Emscripten
    # Avoid "undefined symbol" errors, see: https://stackoverflow.com/a/37869647
    target_link_options(libsf PRIVATE "--bind")
endif()
