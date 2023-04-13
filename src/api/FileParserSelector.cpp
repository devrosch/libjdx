#include "api/FileParserSelector.hpp"

#include <vector>

#ifdef __EMSCRIPTEN__
#include <emscripten/bind.h>
#endif

sciformats::api::FileParserSelector::FileParserSelector(
    std::vector<std::shared_ptr<sciformats::api::FileParser>> fileParsers)
    : m_fileParsers{std::move(fileParsers)}
{
}

bool sciformats::api::FileParserSelector::isRecognized(const std::string& path)
{
    // for C++20 #include <algorithm> and do:
    // std::ranges::any_of(m_fileParsers,
    //     [](auto const& parserPtr) {parserPtr->isRecognized(path)});
    // for c++17, still do:
    // NOLINTNEXTLINE(readability-use-anyofallof)
    for (auto const& parserPtr : m_fileParsers)
    {
        if (parserPtr->isRecognized(path))
        {
            return true;
        }
    }
    return false;
}

std::unique_ptr<sciformats::api::Node>
sciformats::api::FileParserSelector::parse(const std::string& path)
{
    auto parseErrors = std::vector<std::string>{};

    for (auto const& parserPtr : m_fileParsers)
    {
        if (parserPtr->isRecognized(path))
        {
            try
            {
                auto node = parserPtr->parse(path);
                return node;
            }
            catch (const std::exception& ex)
            {
                parseErrors.emplace_back(ex.what());
            }
        }
    }
    if (parseErrors.empty())
    {
        // no suitable file parser was found
        throw std::runtime_error("No suitable parser found for: " + path);
    }
    // suitable file parser(s) found but all threw exceptions while parsing
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
EMSCRIPTEN_BINDINGS(FileParserSelector)
{
    using namespace sciformats::api;
    using namespace emscripten;
    class_<FileParserSelector, base<FileParser>>("FileParserSelector")
        .constructor<
            std::vector<std::shared_ptr<sciformats::api::FileParser>>>()
        .function("isRecognized", &FileParserSelector::isRecognized)
        .function("parse", &FileParserSelector::parse);
    register_vector<std::shared_ptr<sciformats::api::FileParser>>(
        "vector<std::shared_ptr<sciformats::api::FileParser>>");
}
#endif
