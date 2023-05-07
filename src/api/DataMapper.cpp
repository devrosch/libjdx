#include "api/DataMapper.hpp"

#ifdef __EMSCRIPTEN__
#include <emscripten/bind.h>
#endif

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_BINDINGS(DataMapper)
{
    using namespace sciformats::api;
    using namespace emscripten;
    class_<DataMapper>("DataMapper")
        .smart_ptr<std::shared_ptr<DataMapper>>("std::shared_ptr<DataMapper>");
}
#endif
