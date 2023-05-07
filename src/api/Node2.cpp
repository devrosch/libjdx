#include "api/Node2.hpp"

#ifdef __EMSCRIPTEN__
#include <emscripten/bind.h>
#endif

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_BINDINGS(Node2)
{
    using namespace sciformats::api;
    using namespace emscripten;

    value_object<Node2>("Node2")
    .field("name", &Node2::name)
    .field("parameters", &Node2::parameters)
    .field("data", &Node2::data)
    .field("childNodeNames", &Node2::childNodeNames);

    // cannot use unique_ptr in embind
    // see:
    // https://stackoverflow.com/questions/31814092/cant-use-vector-of-unique-ptr-in-emscripten-bindings
    // register_vector<KeyValueParam>("vector<KeyValueParam>");
    // register_vector<Point2D>("vector<Point2D>");
    // register_vector<std::string>("vector<std::string>");
    // as an alternative for mapping vectors to JS arrays, see:
    // https://github.com/emscripten-core/emscripten/issues/11070
}
#endif
