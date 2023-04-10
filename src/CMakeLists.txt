add_subdirectory(api)
add_subdirectory(io)
add_subdirectory(jdx)
add_subdirectory(stub)
add_subdirectory(sf)

set(HEADER_LIST
    # list of header files
    # populated from subdirectiory CMakeList.txt files
    ${API_HEADER_LIST}
    ${IO_HEADER_LIST}
    ${JDX_HEADER_LIST}
    ${STUB_HEADER_LIST}
    ${SF_HEADER_LIST}
)

set(SOURCE_LIST
    # list of source code files (*.cpp)
    # populated from subdirectiory CMakeList.txt files
    ${API_SOURCE_LIST}
    ${IO_SOURCE_LIST}
    ${JDX_SOURCE_LIST}
    ${STUB_SOURCE_LIST}
    ${SF_SOURCE_LIST}
)

# Make an automatic library - will be static or dynamic based on user setting
add_library(sf
    ${SOURCE_LIST}
    ${HEADER_LIST}
)

# All users of this library will need at least C++17
target_compile_features(sf PUBLIC cxx_std_17)
# For g++ 12 the above does not seem to suffice (e.g. does not recognize std::optional), hence additionally setting properties
set_target_properties(sf PROPERTIES
    CXX_STANDARD 17
    CXX_EXTENSIONS OFF
)

if (CMAKE_COMPILER_IS_GNUCXX AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 9)
    # make filesystem available
    # see: https://stackoverflow.com/questions/33149878/experimentalfilesystem-linker-error
    message(STATUS "Linking stdc++fs to GNUCXX version: ${CMAKE_CXX_COMPILER_VERSION}")
    target_link_libraries(sf stdc++fs)
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
    target_link_libraries(sf PRIVATE ICU::uc)
endif()

# public headers
target_include_directories(sf PUBLIC ${PROJECT_SOURCE_DIR}/include)
# private headers
target_include_directories(sf PRIVATE ${PROJECT_SOURCE_DIR}/src/jdx)

# --------------------------------------------------------------
# Use include-what-you-use if available => deactivated due to some iwyu issues
# --------------------------------------------------------------
#find_program(iwyu_path NAMES include-what-you-use iwyu REQUIRED)
# if(NOT iwyu_path)
#   message(FATAL_ERROR "Could not find the program include-what-you-use")
# endif()
#set_property(TARGET jdx PROPERTY CXX_INCLUDE_WHAT_YOU_USE ${iwyu_path})
