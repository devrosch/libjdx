#include <cstdint>
#include <string>
#ifdef __EMSCRIPTEN__
#include <emscripten/bind.h>

/**
 * Helper function to get exception message from JS for Emscripten JS exceptions.
 * See: https://emscripten.org/docs/porting/Debugging.html#handling-c-exceptions-from-javascript
*/
std::string getCppExceptionMessage(std::intptr_t exceptionPtr)
{
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
    return std::string(reinterpret_cast<std::exception*>(exceptionPtr)->what());
}

EMSCRIPTEN_BINDINGS(Bindings)
{
    emscripten::function("getCppExceptionMessage", &getCppExceptionMessage);
};
#endif
