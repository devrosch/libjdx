set(HEADER_LIST
    "${PROJECT_SOURCE_DIR}/include/io/BinaryReader.hpp"
)

# Make an automatic library - will be static or dynamic based on user setting
add_library(binaryreader
    BinaryReader.cpp
    ${HEADER_LIST}
)

# alternatively, checking with "if(${CMAKE_SYSTEM_NAME} MATCHES "[Ee]mscripten")"
if (EMSCRIPTEN)
    message(STATUS "Setting USE_ICU=1 flag for Emscripten build...")
    # TODO: also set USE_ICU through compile/link options
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s USE_ICU=1")
else()
    message(STATUS "Trying to find native ICU package...")
    if (APPLE)
        # see: https://stackoverflow.com/questions/34795816/hinting-findname-cmake-files-with-a-custom-directory
        # https://stackoverflow.com/questions/52510499/the-following-icu-libraries-were-not-found-i18n-required
        # set (ICU_ROOT /usr/local/opt/icu4c)
        list(APPEND CMAKE_PREFIX_PATH "/usr/local/opt/icu4c") # required when installing ICU on macOS via Homebrew
    endif()
    find_package(ICU REQUIRED COMPONENTS uc)
    target_link_libraries(binaryreader PRIVATE ICU::uc)
endif()

# All users of this library will need at least C++17
target_compile_features(binaryreader PUBLIC cxx_std_17)

# We need this directory, and users of our library will need it too
target_include_directories(binaryreader
    PUBLIC "${PROJECT_SOURCE_DIR}/include"
)
