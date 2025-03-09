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
#include "jdx/api/JdxScanner.hpp"
#include "jdx/JdxParser.hpp"
#include "jdx/api/JdxConverter.hpp"

#include <fstream>
#include <iostream>
#include <sstream>

#ifdef __EMSCRIPTEN__
#include <emscripten/bind.h>
#endif

bool libjdx::jdx::api::JdxScanner::isRecognized(const std::string& path)
{
    std::cout << "C++: JdxScanner.isRecognized(): " << path << '\n';
    std::ifstream input{path};
    auto isRecognized = libjdx::jdx::JdxParser::canParse(path, input);
    return isRecognized;
}

std::unique_ptr<libjdx::api::Converter>
libjdx::jdx::api::JdxScanner::getConverter(const std::string& path)
{
    return std::make_unique<libjdx::jdx::api::JdxConverter>(path);
}

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_BINDINGS(JdxScanner)
{
    using namespace libjdx::api;
    using namespace libjdx::jdx::api;
    using namespace emscripten;
    class_<JdxScanner, base<Scanner>>("JdxScanner")
        .smart_ptr_constructor("JdxScanner", &std::make_shared<JdxScanner>)
        .function("isRecognized", &JdxScanner::isRecognized)
        .function("getConverter", &JdxScanner::getConverter);
}
#endif
