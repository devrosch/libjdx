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
