#include "api/Converter.hpp"

#ifdef __EMSCRIPTEN__
#include <emscripten/bind.h>
#endif

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_BINDINGS(Converter)
{
    using namespace sciformats::api;
    using namespace emscripten;
    class_<Converter>("Converter")
        .smart_ptr<std::shared_ptr<Converter>>("std::shared_ptr<Converter>");
}
#endif
