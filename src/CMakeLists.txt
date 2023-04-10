# libsf
add_library("${SF_LIBRARY_NAME}")

add_subdirectory(api)
add_subdirectory(io)
add_subdirectory(jdx)
add_subdirectory(stub)
add_subdirectory(sf)

# All users of this library will need at least C++17
target_compile_features("${SF_LIBRARY_NAME}" PUBLIC cxx_std_17)
# For g++ 12 the above does not seem to suffice (e.g. does not recognize std::optional), hence additionally setting properties
set_target_properties("${SF_LIBRARY_NAME}" PROPERTIES
    CXX_EXTENSIONS OFF
    CXX_STANDARD 17
    CMAKE_CXX_STANDARD_REQUIRED ON
)

if (CMAKE_COMPILER_IS_GNUCXX AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 9)
    # make filesystem available
    # see: https://stackoverflow.com/questions/33149878/experimentalfilesystem-linker-error
    message(STATUS "Linking stdc++fs to GNUCXX version: ${CMAKE_CXX_COMPILER_VERSION}")
    target_link_libraries("${SF_LIBRARY_NAME}" stdc++fs)
endif()

if (EMSCRIPTEN)
    message(STATUS "Setting USE_ICU=1 flag for Emscripten build...")
    # TODO: also set USE_ICU through compile/link options
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s USE_ICU=1")
    # make sure that code not used in this target is retained, see: https://emscripten.org/docs/porting/connecting_cpp_and_javascript/embind.html
    # for making this work in CMake, see: https://stackoverflow.com/a/59994837
    # alternative discussion https://stackoverflow.com/questions/53071878/using-whole-archive-linker-option-with-cmake-and-libraries-with-other-library
    target_link_options(sf INTERFACE "LINKER:--whole-archive")
    # Avoid "undefined symbol" errors in consumers, see: https://stackoverflow.com/a/37869647
    target_link_options(sf INTERFACE "--bind")
else()
    message(STATUS "Trying to find native ICU package...")
    if (APPLE)
        # see: https://stackoverflow.com/questions/34795816/hinting-findname-cmake-files-with-a-custom-directory
        # https://stackoverflow.com/questions/52510499/the-following-icu-libraries-were-not-found-i18n-required
        # set (ICU_ROOT /usr/local/opt/icu4c)
        list(APPEND CMAKE_PREFIX_PATH "/usr/local/opt/icu4c") # required when installing ICU on macOS via Homebrew
    endif()
    find_package(ICU REQUIRED COMPONENTS uc)
    target_link_libraries("${SF_LIBRARY_NAME}" PRIVATE ICU::uc)
endif()

# public headers
target_include_directories("${SF_LIBRARY_NAME}" PUBLIC ${PROJECT_SOURCE_DIR}/include)
# private headers
target_include_directories("${SF_LIBRARY_NAME}" PRIVATE ${PROJECT_SOURCE_DIR}/src/jdx)

# --------------------------------------------------------------
# Use include-what-you-use if available => deactivated due to some iwyu issues
# --------------------------------------------------------------
#find_program(iwyu_path NAMES include-what-you-use iwyu REQUIRED)
# if(NOT iwyu_path)
#   message(FATAL_ERROR "Could not find the program include-what-you-use")
# endif()
#set_property(TARGET "${SF_LIBRARY_NAME}" PROPERTY CXX_INCLUDE_WHAT_YOU_USE ${iwyu_path})
