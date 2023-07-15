#ifndef API_CONVERTERSERVICE_HPP
#define API_CONVERTERSERVICE_HPP

#include "api/Converter.hpp"
#include "api/Scanner.hpp"

#include <string>
#include <vector>

namespace sciformats::api
{
class ConverterService : public Scanner
{
public:
    explicit ConverterService(std::vector<std::shared_ptr<Scanner>> scanners);
    bool isRecognized(const std::string& path) override;
    std::unique_ptr<Converter> getConverter(const std::string& path) override;

private:
    std::vector<std::shared_ptr<Scanner>> m_scanners;
};
} // sciformats::api

#endif
