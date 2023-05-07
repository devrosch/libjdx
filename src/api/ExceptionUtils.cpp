#include <string>
#include <cstdint>
#ifdef __EMSCRIPTEN__
#include <emscripten/bind.h>
#endif

std::string getExceptionMessage(std::intptr_t exceptionPtr) {
  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
  return std::string(reinterpret_cast<std::exception*>(exceptionPtr)->what());
}

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_BINDINGS(Bindings) {
  emscripten::function("getExceptionMessage", &getExceptionMessage);
};
#endif
