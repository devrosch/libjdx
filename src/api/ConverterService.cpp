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
#include "api/ConverterService.hpp"

#include <vector>

#ifdef __EMSCRIPTEN__
#include <emscripten/bind.h>
#endif

sciformats::api::ConverterService::ConverterService(
    std::vector<std::shared_ptr<sciformats::api::Scanner>> scanners)
    : m_scanners{std::move(scanners)}
{
}

bool sciformats::api::ConverterService::isRecognized(const std::string& path)
{
    // for C++20 #include <algorithm> and do:
    // std::ranges::any_of(m_fileParsers,
    //     [](auto const& scannerPtr) {scannerPtr->isRecognized(path)});
    // for c++17, still do:
    // NOLINTNEXTLINE(readability-use-anyofallof)
    for (auto const& scannerPtr : m_scanners)
    {
        if (scannerPtr->isRecognized(path))
        {
            return true;
        }
    }
    return false;
}

std::unique_ptr<sciformats::api::Converter>
sciformats::api::ConverterService::getConverter(const std::string& path)
{
    auto parseErrors = std::vector<std::string>{};

    for (auto const& scannerPtr : m_scanners)
    {
        if (scannerPtr->isRecognized(path))
        {
            try
            {
                auto converterPtr = scannerPtr->getConverter(path);
                return converterPtr;
            }
            catch (const std::exception& ex)
            {
                parseErrors.emplace_back(ex.what());
            }
        }
    }
    if (parseErrors.empty())
    {
        // no suitable converter was found
        throw std::runtime_error("No suitable converter found for: " + path);
    }
    // suitable converter(s) found but all threw exceptions while parsing
    // => collect exception messages and throw new umbrella exception
    std::string message{"Errors encountered while parsing: " + path + '\n'};
    size_t i = 0;
    for (auto const& error : parseErrors)
    {
        message.append("Parser " + std::to_string(++i) + ": ")
            .append(error)
            .append("\n");
    }
    throw std::runtime_error(message);
}

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_BINDINGS(ConverterService)
{
    using namespace sciformats::api;
    using namespace emscripten;
    class_<ConverterService, base<Scanner>>("ConverterService")
        .constructor<std::vector<std::shared_ptr<sciformats::api::Scanner>>>()
        .function("isRecognized", &ConverterService::isRecognized)
        .function("getConverter", &ConverterService::getConverter);
    register_vector<std::shared_ptr<sciformats::api::Scanner>>(
        "vector<std::shared_ptr<sciformats::api::Scanner>>");
}
#endif
