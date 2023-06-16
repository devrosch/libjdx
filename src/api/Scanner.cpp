#include "api/Scanner.hpp"

#ifdef __EMSCRIPTEN__
#include <emscripten/bind.h>
#endif

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_BINDINGS(Scanner)
{
    using namespace sciformats::api;
    using namespace emscripten;
    class_<Scanner>("Scanner")
        .smart_ptr<std::shared_ptr<Scanner>>(
            "std::shared_ptr<Scanner>");
}
#endif
