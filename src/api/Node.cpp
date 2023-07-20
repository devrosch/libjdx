#include "api/Node.hpp"

#ifdef __EMSCRIPTEN__
#include <emscripten/bind.h>
#endif

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_BINDINGS(Node)
{
    using namespace sciformats::api;
    using namespace emscripten;

    value_object<Node>("Node")
        .field("name", &Node::name)
        .field("parameters", &Node::parameters)
        .field("data", &Node::data)
        .field("peakTable", &Node::peakTable)
        .field("childNodeNames", &Node::childNodeNames);

    // cannot use unique_ptr in embind
    // see:
    // https://stackoverflow.com/questions/31814092/cant-use-vector-of-unique-ptr-in-emscripten-bindings
    register_vector<KeyValueParam>("vector<KeyValueParam>");
    register_vector<Point2D>("vector<Point2D>");
    register_vector<std::string>("vector<std::string>");
    // as an alternative for mapping vectors to JS arrays, see:
    // https://github.com/emscripten-core/emscripten/issues/11070
}
#endif
