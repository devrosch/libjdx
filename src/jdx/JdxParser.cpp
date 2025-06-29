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
// determine the availability of the filesystem header
// inspired by:
// https://stackoverflow.com/questions/53365538/how-to-determine-whether-to-use-filesystem-or-experimental-filesystem
#ifndef JDX_USE_EXPERIMENTAL_FILESYSTEM
#if defined(__cpp_lib_filesystem)
// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define JDX_USE_EXPERIMENTAL_FILESYSTEM 0
#elif defined(__cpp_lib_experimental_filesystem)
// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define JDX_USE_EXPERIMENTAL_FILESYSTEM 1
#elif !defined(__has_include)
// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define JDX_USE_EXPERIMENTAL_FILESYSTEM 1
#elif __has_include(<filesystem>)
// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define JDX_USE_EXPERIMENTAL_FILESYSTEM 0
#elif __has_include(<experimental/filesystem>)
// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define JDX_USE_EXPERIMENTAL_FILESYSTEM 1
#endif
#endif

#ifndef JDX_USE_EXPERIMENTAL_FILESYSTEM
#error Required <filesystem> header not available.
#endif

#include "jdx/JdxParser.hpp"
#include "io/TextReader.hpp"
#include "util/LdrUtils.hpp"
#include "util/StringUtils.hpp"

#include <algorithm>
#include <array>
#include <climits>
#include <cstring>
#include <limits>
#include <sstream>
// include and alias filesystem header
#if JDX_USE_EXPERIMENTAL_FILESYSTEM
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;
#else
#include <filesystem>
namespace fs = std::filesystem;
#endif

bool libjdx::jdx::JdxParser::canParse(
    const std::string& filePath, std::istream& iStream)
{
    // check extension
    std::string extension = fs::path(filePath).extension();
    util::toLower(extension);
    if (std::find(std::begin(s_acceptedExtensions),
            std::end(s_acceptedExtensions), extension)
        == std::end(s_acceptedExtensions))
    {
        return false;
    }

    // read first few chars
    std::ios::pos_type position = iStream.tellg();
    std::string buffer(s_scanBufferSize, '\0');
    iStream.read(buffer.data(), s_scanBufferSize);
    const auto bytesRead = iStream.gcount();
    if (iStream.eof() || iStream.fail() || iStream.bad())
    {
        iStream.clear();
    }
    iStream.seekg(position, std::ios_base::beg);
    buffer.resize(bytesRead);
    if (util::isLdrStart(buffer))
    {
        auto label = util::parseLdrStart(buffer).first;
        return label == "TITLE";
    }
    return false;
}

libjdx::jdx::Block libjdx::jdx::JdxParser::parse(
    std::unique_ptr<std::istream> streamPtr)
{
    auto textReaderPtr = std::make_unique<io::TextReader>(std::move(streamPtr));
    libjdx::jdx::Block block{std::move(textReaderPtr)};
    return block;
}

libjdx::jdx::Block libjdx::jdx::JdxParser::parse(const std::string& filePath)
{
    auto textReaderPtr = std::make_unique<io::TextReader>(filePath);
    libjdx::jdx::Block block{std::move(textReaderPtr)};
    return block;
}
