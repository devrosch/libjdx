#include "jdx/api/JdxDataScanner.hpp"
#include "api/Node.hpp"
#include "jdx/JdxParser.hpp"
#include "jdx/api/JdxBlockNode.hpp"

#include <fstream>
#include <iostream>
#include <sstream>

#ifdef __EMSCRIPTEN__
#include <emscripten/bind.h>
#endif

bool sciformats::jdx::api::JdxDataScanner::isRecognized(const std::string& path)
{
    std::cout << "C++: JdxDataScanner.isRecognized(): " << path << '\n';
    std::ifstream input{path};
    auto isRecognized = sciformats::jdx::JdxParser::canParse(path, input);
    return isRecognized;
}

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_BINDINGS(JdxDataScanner)
{
    using namespace sciformats::api;
    using namespace sciformats::jdx::api;
    using namespace emscripten;
    class_<JdxDataScanner, base<DataScanner>>("JdxDataScanner")
        .smart_ptr_constructor(
            "JdxDataScanner", &std::make_shared<JdxDataScanner>)
        .function("isRecognized", &JdxDataScanner::isRecognized);
}
#endif
