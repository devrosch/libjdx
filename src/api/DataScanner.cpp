#include "api/DataScanner.hpp"

#ifdef __EMSCRIPTEN__
#include <emscripten/bind.h>
#endif

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_BINDINGS(DataScanner)
{
    using namespace sciformats::api;
    using namespace emscripten;
    class_<DataScanner>("DataScanner")
        .smart_ptr<std::shared_ptr<DataScanner>>("std::shared_ptr<DataScanner>");
}
#endif
