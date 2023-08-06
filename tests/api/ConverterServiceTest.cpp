#define CATCH_CONFIG_MAIN
#include "api/ConverterService.hpp"
#include "api/Converter.hpp"
#include "api/Node.hpp"

#include "catch2/catch.hpp"
#include "catch2/trompeloeil.hpp"

// -Wweak-vtable warning due to QTCreator bug, see:
// https://stackoverflow.com/questions/50463374/avoid-weak-vtable-warnings-for-classes-only-defined-in-a-source-file
// , also: https://bugreports.qt.io/browse/QTCREATORBUG-19741
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wweak-vtables"
#endif
class MockScanner : public trompeloeil::mock_interface<sciformats::api::Scanner>
{
    IMPLEMENT_MOCK1(isRecognized);
    IMPLEMENT_MOCK1(getConverter);
};

class SuccessConverter : public sciformats::api::Converter
{
public:
    explicit SuccessConverter(std::string nodeName)
        : m_nodeName{std::move(nodeName)}
    {
    }
    sciformats::api::Node read(const std::string& path) override
    {
        return {m_nodeName, std::vector<sciformats::api::KeyValueParam>{},
            std::vector<sciformats::api::Point2D>{},
            std::map<std::string, std::string>{}, sciformats::api::Table{},
            std::vector<std::string>{}};
    }

private:
    std::string m_nodeName;
};

class ErrorConverter : public sciformats::api::Converter
{
public:
    explicit ErrorConverter(std::string errorMessage)
        : m_errorMessage{std::move(errorMessage)}
    {
        throw std::runtime_error(m_errorMessage);
    }
    sciformats::api::Node read(const std::string& path) override
    {
        return {};
    }

private:
    std::string m_errorMessage;
};
#ifdef __clang__
#pragma clang diagnostic pop
#endif

TEST_CASE("ConverterService sequentially checks parsers for applicability",
    "[ConverterService]")
{
    using namespace sciformats::api;

    auto mockScannerPtr0 = std::make_shared<MockScanner>();
    REQUIRE_CALL(*mockScannerPtr0, isRecognized(ANY(const std::string&)))
        .TIMES(1)
        .RETURN(false);

    auto mockScannerPtr1 = std::make_shared<MockScanner>();
    REQUIRE_CALL(*mockScannerPtr1, isRecognized(ANY(const std::string&)))
        .TIMES(1)
        .RETURN(true);

    auto mockScannerPtr2 = std::make_shared<MockScanner>();
    REQUIRE_CALL(*mockScannerPtr2, isRecognized(ANY(const std::string&)))
        .TIMES(0);

    ConverterService service{
        {mockScannerPtr0, mockScannerPtr1, mockScannerPtr2}};

    REQUIRE(service.isRecognized("resources/dummy.txt"));
}

TEST_CASE("ConverterService collects failed parsing error messages",
    "[ConverterService]")
{
    using namespace sciformats::api;

    auto mockScannerPtr0 = std::make_shared<MockScanner>();
    REQUIRE_CALL(*mockScannerPtr0, isRecognized(ANY(const std::string&)))
        .TIMES(1)
        .RETURN(true);
    REQUIRE_CALL(*mockScannerPtr0, getConverter(ANY(const std::string&)))
        .TIMES(1)
        .RETURN(std::make_unique<ErrorConverter>("Error 1"));

    auto mockScannerPtr1 = std::make_shared<MockScanner>();
    REQUIRE_CALL(*mockScannerPtr1, isRecognized(ANY(const std::string&)))
        .TIMES(1)
        .RETURN(false);
    REQUIRE_CALL(*mockScannerPtr1, getConverter(ANY(const std::string&)))
        .TIMES(0);

    auto mockScannerPtr2 = std::make_shared<MockScanner>();
    REQUIRE_CALL(*mockScannerPtr2, isRecognized(ANY(const std::string&)))
        .TIMES(1)
        .RETURN(true);
    REQUIRE_CALL(*mockScannerPtr2, getConverter(ANY(const std::string&)))
        .TIMES(1)
        // there is no obvious way to return a unique_ptr of a mock object,
        // hence use Stub
        .RETURN(std::make_unique<ErrorConverter>("Error 3"));

    ConverterService service{
        {mockScannerPtr0, mockScannerPtr1, mockScannerPtr2}};

    REQUIRE_THROWS_WITH(service.getConverter("resources/dummy.txt"),
        Catch::Matchers::Contains("Error 1")
            && Catch::Matchers::Contains("Error 3"));
}

TEST_CASE("ConverterService provides generic error when no suitable parser "
          "is found",
    "[ConverterService]")
{
    using namespace sciformats::api;

    auto mockScannerPtr0 = std::make_shared<MockScanner>();
    REQUIRE_CALL(*mockScannerPtr0, isRecognized(ANY(const std::string&)))
        .TIMES(1)
        .RETURN(false);

    auto mockScannerPtr1 = std::make_shared<MockScanner>();
    REQUIRE_CALL(*mockScannerPtr1, isRecognized(ANY(const std::string&)))
        .TIMES(1)
        .RETURN(false);

    ConverterService service{{mockScannerPtr0, mockScannerPtr1}};

    REQUIRE_THROWS_WITH(service.getConverter("resources/dummy.txt"),
        Catch::Matchers::Contains(
            "No suitable converter", Catch::CaseSensitive::No));
}

TEST_CASE(
    "ConverterService shallow check returns false when no suitable parser "
    "is found",
    "[ConverterService]")
{
    using namespace sciformats::api;

    auto mockScannerPtr0 = std::make_shared<MockScanner>();
    REQUIRE_CALL(*mockScannerPtr0, isRecognized(ANY(const std::string&)))
        .TIMES(1)
        .RETURN(false);

    ConverterService service{{mockScannerPtr0}};

    REQUIRE_FALSE(service.isRecognized("resources/dummy.txt"));
}

TEST_CASE("ConverterService returns converter from first applicable scanner",
    "[ConverterService]")
{
    using namespace sciformats::api;

    const auto* nodeName = "Test node name";

    auto mockScannerPtr0 = std::make_shared<MockScanner>();
    REQUIRE_CALL(*mockScannerPtr0, isRecognized(ANY(const std::string&)))
        .TIMES(1)
        .RETURN(true);
    REQUIRE_CALL(*mockScannerPtr0, getConverter(ANY(const std::string&)))
        .TIMES(1)
        // there is no obvious way to return a unique_ptr of a mock object,
        // hence use Stub
        .RETURN(std::make_unique<SuccessConverter>(nodeName));

    auto mockScannerPtr1 = std::make_shared<MockScanner>();
    REQUIRE_CALL(*mockScannerPtr1, isRecognized(ANY(const std::string&)))
        .TIMES(0);
    REQUIRE_CALL(*mockScannerPtr1, getConverter(ANY(const std::string&)))
        .TIMES(0);

    ConverterService service{{mockScannerPtr0, mockScannerPtr1}};

    auto converter = service.getConverter("resources/dummy.txt");

    REQUIRE(converter != nullptr);
    REQUIRE(nodeName == converter->read("/").name);
}
