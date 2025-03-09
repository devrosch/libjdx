/* Copyright (C) 2025 Robert Schiwon
 *
 * This file is part of libjdx.
 *
 * libjdx is free software: you can redistribute it and/or modify it under the
 * terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option) any
 * later version.
 *
 * libjdx is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with libjdx. If not, see <https://www.gnu.org/licenses/>.
 */
#include <cstdint>
#include <string>
#ifdef __EMSCRIPTEN__
#include <emscripten/bind.h>

/**
 * Helper function to get exception message from JS for Emscripten JS
 * exceptions. See:
 * https://emscripten.org/docs/porting/Debugging.html#handling-c-exceptions-from-javascript
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
