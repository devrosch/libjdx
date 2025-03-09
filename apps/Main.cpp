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
#include <iostream>

#ifdef JDX_PROJECT_NAME
constexpr const char* projectName = JDX_PROJECT_NAME;
#else
constexpr const char* projectName = "<unknown name>";
#endif

#ifdef JDX_PROJECT_VERSION
constexpr const char* projectVersion = JDX_PROJECT_VERSION;
#else
constexpr const char* projectVersion = "<unknown version>";
#endif

int main()
{
    std::cout << "C++: " << projectName << " " << projectVersion
              << " started\n";
    // noop, required for emscripten, so that *.js and *.wasm files get
    // generated for library configured as dependency see:
    // https://stackoverflow.com/questions/34234446/cmake-is-it-possible-to-build-an-executable-from-only-static-libraries-and-no-s
}
