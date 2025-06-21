#include "../exceptions/argument_parser_exceptions.h"
#include "../utils/argument_parser.h"
#include "catch2/catch_test_macros.hpp"

namespace {
void TestWithArguments(const std::vector<FilterArguments> &expected_arguments, const std::string &expected_input_path,
                       const std::string &expected_output_path, const std::vector<std::string> &arguments) {
    std::vector<FilterArguments> result_arguments{};
    std::string result_input_path;
    std::string result_output_path;

    ArgumentParser::ParseArguments(arguments, result_input_path, result_output_path, result_arguments);

    REQUIRE(result_arguments == expected_arguments);
    REQUIRE(result_input_path == expected_input_path);
    REQUIRE(result_output_path == expected_output_path);
}

template <typename T>
void TestWithBadArguments(const std::vector<std::string> &arguments) {
    std::vector<FilterArguments> result_arguments{};
    std::string result_input_path;
    std::string result_output_path;

    REQUIRE_THROWS_AS(
        ArgumentParser::ParseArguments(arguments, result_input_path, result_output_path, result_arguments), T);
}

}  // namespace

TEST_CASE("Valid") {
    const std::string program_name = "./image_processor";

    std::vector<FilterArguments> expected_arguments{};
    std::string expected_input_path = "input.bmp";
    std::string expected_output_path = "output.bmp";
    std::vector<std::string> arguments{program_name, "input.bmp", "output.bmp"};

    TestWithArguments(expected_arguments, expected_input_path, expected_output_path, arguments);

    expected_arguments = {FilterArguments("-crop", {})};
    expected_input_path = "input.bmp";
    expected_output_path = "output.bmp";
    arguments = {program_name, "input.bmp", "output.bmp", "-crop"};

    TestWithArguments(expected_arguments, expected_input_path, expected_output_path, arguments);

    expected_arguments = {FilterArguments("-crop", {124, 431, -1221}), FilterArguments("-gs", {})};  // NOLINT
    expected_input_path = "input.bmp";
    expected_output_path = "output.bmp";
    arguments = {program_name, "input.bmp", "output.bmp", "-crop", "124", "431", "-1221", "-gs"};

    TestWithArguments(expected_arguments, expected_input_path, expected_output_path, arguments);

    expected_arguments = {FilterArguments("-crop1231", {124, 431, -12189183921}), FilterArguments("-gs", {}),  // NOLINT
                          FilterArguments("-shades", {}), FilterArguments("-ksdghrfywifd", {})};
    expected_input_path = "input.bmp";
    expected_output_path = "output.bmp";
    arguments = {program_name, "input.bmp",    "output.bmp", "-crop1231", "124",
                 "431",        "-12189183921", "-gs",        "-shades",   "-ksdghrfywifd"};

    TestWithArguments(expected_arguments, expected_input_path, expected_output_path, arguments);

    expected_arguments = {FilterArguments("-crop", {12.13, -12.12222222})};  // NOLINT
    expected_input_path = "input.bmp";
    expected_output_path = "output.bmp";
    arguments = {program_name, "input.bmp", "output.bmp", "-crop", "12.13", "-12.12222222"};

    TestWithArguments(expected_arguments, expected_input_path, expected_output_path, arguments);
}

TEST_CASE("Bad file format") {
    const std::string program_name = "./image_processor";

    std::vector<std::string> arguments{program_name, "input.txt", "output.bmp"};

    TestWithBadArguments<argument_parser_exceptions::BadFileFormatException>(arguments);

    arguments = {program_name, "input", "output.bmp"};

    TestWithBadArguments<argument_parser_exceptions::BadFileFormatException>(arguments);

    arguments = {program_name, "inpadjsgheripasbghnweut", "outputskdjfwejkfuwafesa"};

    TestWithBadArguments<argument_parser_exceptions::BadFileFormatException>(arguments);

    arguments = {program_name, "inpadjsgheripasbghnweut.bmp", "outputskdjfwejkfuwafes.a.bmp2"};

    TestWithBadArguments<argument_parser_exceptions::BadFileFormatException>(arguments);

    arguments = {
        program_name, "inpadjsgheripasbghnweut.bmp", "outputskdjfwejkfuwafes.a.bmp2", "-crop", "190654081`23", "-gs",
        "1287301"};

    TestWithBadArguments<argument_parser_exceptions::BadFileFormatException>(arguments);
}

TEST_CASE("Not enough arguments") {
    const std::string program_name = "./image_processor";

    std::vector<std::string> arguments{program_name};

    TestWithBadArguments<argument_parser_exceptions::NotEnoughArgumentsException>(arguments);

    arguments = {program_name, "isgAWDlksafnWE"};

    TestWithBadArguments<argument_parser_exceptions::NotEnoughArgumentsException>(arguments);

    arguments = {};

    TestWithBadArguments<argument_parser_exceptions::NotEnoughArgumentsException>(arguments);

    arguments = {"18273912", "jkldsfh"};

    TestWithBadArguments<argument_parser_exceptions::NotEnoughArgumentsException>(arguments);
}

TEST_CASE("Bad filter name") {
    const std::string program_name = "./image_processor";

    std::vector<std::string> arguments{program_name, "input.bmp", "output.bmp", "color"};

    TestWithBadArguments<argument_parser_exceptions::BadFilterNameException>(arguments);

    arguments = {program_name, "input.bmp", "output.bmp", "123"};

    TestWithBadArguments<argument_parser_exceptions::BadFilterNameException>(arguments);

    arguments = {program_name, "input.bmp", "output.bmp", "-"};

    TestWithBadArguments<argument_parser_exceptions::BadFilterNameException>(arguments);

    arguments = {program_name, "input.bmp", "output.bmp", "-a%&$@!&"};

    TestWithBadArguments<argument_parser_exceptions::BadFilterNameException>(arguments);

    arguments = {program_name, "input.bmp", "output.bmp", "-%&$@!&"};

    TestWithBadArguments<argument_parser_exceptions::BadFilterNameException>(arguments);
}

TEST_CASE("Bad filter parameter") {
    const std::string program_name = "./image_processor";

    std::vector<std::string> arguments{program_name, "input.bmp", "output.bmp", "-color", "123,123"};

    TestWithBadArguments<argument_parser_exceptions::BadFilterParameterException>(arguments);

    arguments = {program_name, "input.bmp", "output.bmp", "-crop", "0crop"};

    TestWithBadArguments<argument_parser_exceptions::BadFilterParameterException>(arguments);

    arguments = {program_name, "input.bmp", "output.bmp", "-crop", "12.123&&"};

    TestWithBadArguments<argument_parser_exceptions::BadFilterParameterException>(arguments);

    arguments = {program_name, "input.bmp", "output.bmp", "-crop", "27379.123.123"};

    TestWithBadArguments<argument_parser_exceptions::BadFilterParameterException>(arguments);

    arguments = {
        program_name,
        "input.bmp",
        "output.bmp",
        "-crop",
        "-gs",
        "-9999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999"
        "9"
        "99999999999999999999999999999999999999999999999999999999999999999999999999999."
        "999999999999999999999999999999999"
        "99999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999"
        "9999999999999999999999999999.99"};

    TestWithBadArguments<argument_parser_exceptions::BadFilterParameterException>(arguments);

    arguments = {program_name, "input.bmp", "output.bmp", "-crop", "&*^%#&123123"};

    TestWithBadArguments<argument_parser_exceptions::BadFilterParameterException>(arguments);

    arguments = {program_name, "input.bmp", "output.bmp", "-crop", "*&139289012"};

    TestWithBadArguments<argument_parser_exceptions::BadFilterParameterException>(arguments);

    arguments = {program_name, "input.bmp", "output.bmp", "-crop", "121-2.8172321"};

    TestWithBadArguments<argument_parser_exceptions::BadFilterParameterException>(arguments);
}

TEST_CASE("Too large filter parameter") {
    const std::string program_name = "./image_processor";

    std::vector<std::string> arguments{
        program_name, "input.bmp", "output.bmp", "-color",
        "99999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999"
        "99999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999"
        "99999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999"
        "999999999999999999999999999999"};

    TestWithBadArguments<argument_parser_exceptions::TooLargeFilterParameterException>(arguments);

    arguments = {
        program_name,
        "input.bmp",
        "output.bmp",
        "-crop",
        "-gs",
        "-9999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999"
        "9"
        "99999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999"
        "99999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999"
        "999999999999999999999999999999"};

    TestWithBadArguments<argument_parser_exceptions::TooLargeFilterParameterException>(arguments);

    arguments = {
        program_name,
        "input.bmp",
        "output.bmp",
        "-crop",
        "-gs",
        "-9999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999"
        "9"
        "99999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999"
        "99999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999"
        "9999999999999999999999999999.99,"};

    TestWithBadArguments<argument_parser_exceptions::TooLargeFilterParameterException>(arguments);
}
