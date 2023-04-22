#include <iostream>

#ifdef SF_PROJECT_NAME
    constexpr const char* projectName = SF_PROJECT_NAME;
#else
    constexpr const char* projectName = "<unknown name>";
#endif

#ifdef SF_PROJECT_VERSION
    constexpr const char* projectVersion = SF_PROJECT_VERSION;
#else
    constexpr const char* projectVersion = "<unknown version>";
#endif

int main()
{
    std::cout << "C++: " << projectName << " " << projectVersion << " started\n";
    // noop, required for emscripten, so that *.js and *.wasm files get
    // generated for library configured as dependency see:
    // https://stackoverflow.com/questions/34234446/cmake-is-it-possible-to-build-an-executable-from-only-static-libraries-and-no-s
}
