#include <string>
#include <stdint.h>
#ifdef __EMSCRIPTEN__
#include <emscripten/bind.h>
#endif

std::string getExceptionMessage(intptr_t exceptionPtr) {
  return std::string(reinterpret_cast<std::exception*>(exceptionPtr)->what());
}

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_BINDINGS(Bindings) {
  emscripten::function("getExceptionMessage", &getExceptionMessage);
};
#endif
