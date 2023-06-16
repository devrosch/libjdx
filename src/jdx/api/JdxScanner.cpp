#include "jdx/api/JdxScanner.hpp"
#include "jdx/api/JdxConverter.hpp"
#include "jdx/JdxParser.hpp"

#include <fstream>
#include <iostream>
#include <sstream>

#ifdef __EMSCRIPTEN__
#include <emscripten/bind.h>
#endif

bool sciformats::jdx::api::JdxScanner::isRecognized(const std::string& path)
{
    std::cout << "C++: JdxScanner.isRecognized(): " << path << '\n';
    std::ifstream input{path};
    auto isRecognized = sciformats::jdx::JdxParser::canParse(path, input);
    return isRecognized;
}

std::unique_ptr<sciformats::api::Converter> sciformats::jdx::api::JdxScanner::getConverter(const std::string& path)
{
    return std::make_unique<sciformats::jdx::api::JdxConverter>(path);
}

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_BINDINGS(JdxScanner)
{
    using namespace sciformats::api;
    using namespace sciformats::jdx::api;
    using namespace emscripten;
    class_<JdxScanner, base<Scanner>>("JdxScanner")
        .smart_ptr_constructor(
            "JdxScanner", &std::make_shared<JdxScanner>)
        .function("isRecognized", &JdxScanner::isRecognized)
        .function("getConverter", &JdxScanner::getConverter);
}
#endif
