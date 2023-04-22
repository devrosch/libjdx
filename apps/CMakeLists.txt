# For WASM, create an executable of same name as library
# Only this will generate *.wasm and *.js files (e.g., libsf.wasm and libsf.js) that wrap the library and can be uses from JS
add_executable("${CMAKE_PROJECT_NAME}"
    Main.cpp
)

# Should be linked to the library
target_link_libraries("${CMAKE_PROJECT_NAME}" PUBLIC "${SF_LIBRARY_NAME}")

target_compile_definitions("${CMAKE_PROJECT_NAME}" PRIVATE SF_PROJECT_NAME="${PROJECT_NAME}")
target_compile_definitions("${CMAKE_PROJECT_NAME}" PRIVATE SF_PROJECT_VERSION="${PROJECT_VERSION}")

if (EMSCRIPTEN)
    # Emscripten
    # Avoid "undefined symbol" errors, see: https://stackoverflow.com/a/37869647
    target_link_options(libsf PRIVATE "--bind")
    # Link workerfs
    target_link_options(libsf PRIVATE "-lworkerfs.js")
endif()
