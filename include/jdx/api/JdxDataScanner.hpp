#ifndef JDX_API_JDXDATASCANNER_HPP
#define JDX_API_JDXDATASCANNER_HPP

#include "api/DataScanner.hpp"

namespace sciformats::jdx::api
{
class JdxDataScanner : public sciformats::api::DataScanner
{
public:
    bool isRecognized(const std::string& path) override;

private:
};
} // sciformats::jdx::api

#endif
