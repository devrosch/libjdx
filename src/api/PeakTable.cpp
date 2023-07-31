#include "api/PeakTable.hpp"

#ifdef __EMSCRIPTEN__
#include <emscripten/bind.h>
#endif

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_BINDINGS(PeakTable)
{
    using namespace sciformats::api;
    using namespace emscripten;

    value_object<PeakTable>("PeakTable")
        .field("columnNames", &PeakTable::columnNames)
        .field("peaks", &PeakTable::peaks);

    value_object<std::pair<std::string, std::string>>(
        "std::pair<std::string, std::string>")
        .field("first", &std::pair<std::string, std::string>::first)
        .field("second", &std::pair<std::string, std::string>::second);
    register_vector<std::pair<std::string, std::string>>(
        "vector<std::pair<std::string, std::string>>");
    register_map<std::string, std::string>("map<std::string, std::string>");
    register_vector<std::map<std::string, std::string>>(
        "vector<std::map<std::string, std::string>>");
}
#endif
