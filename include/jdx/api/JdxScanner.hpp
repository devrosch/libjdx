#ifndef JDX_API_JDXDATASCANNER_HPP
#define JDX_API_JDXDATASCANNER_HPP

#include "api/Scanner.hpp"

namespace sciformats::jdx::api
{
class JdxScanner : public sciformats::api::Scanner
{
public:
    bool isRecognized(const std::string& path) override;
    std::unique_ptr<sciformats::api::Converter> getConverter(
        const std::string& path) override;

private:
};
} // sciformats::jdx::api

#endif
