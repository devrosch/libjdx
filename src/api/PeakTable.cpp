#include "api/PeakTable.hpp"

#ifdef __EMSCRIPTEN__
#include <emscripten/bind.h>
#endif

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_BINDINGS(Node)
{
    using namespace sciformats::api;
    using namespace emscripten;

    value_object<PeakTable>("PeakTable")
        .field("columnNames", &PeakTable::columnNames)
        .field("peaks", &PeakTable::peaks);

    register_vector<std::pair<std::string, std::string>>(
        "vector<std::pair<std::string, std::string>>");
    register_map<std::string, std::string>("map<std::string, std::string>");
    register_vector<std::map<std::string, std::string>>(
        "vector<std::map<std::string, std::string>>");
}
#endif
