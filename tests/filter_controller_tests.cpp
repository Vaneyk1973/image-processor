#include "../exceptions/filter_controller_exceptions.h"
#include "../filters/filter_controller.h"
#include "../filters/filters.h"
#include "../io_handlers/bmp_handler.h"

#include <catch.hpp>

TEST_CASE("Valid") {
    CropFilter crop(0, 0);
    NegativeFilter neg;
    GrayscaleFilter gs;
    SharpeningFilter sharp;
    EdgeDetectionFilter edge(0);
    GaussianBlurFilter blur(0);
    OctogonalizeFilter oct(1);

    std::string input_path =
        "/home/user/ami-236-1-Ivan-Khmura-Vaneyk1973/tasks/image_processor/test_script/data/lenna.bmp";
    BMP origin = BMPHandler::ReadFromFile(input_path);

    BMP expected_file = origin;
    BMP result_file = origin;
    std::vector<FilterArguments> arguments = {};
    FilterController controller(arguments);

    controller.ApplyFilters(result_file.GetImage());

    REQUIRE(expected_file == result_file);

    expected_file = origin;
    result_file = origin;
    arguments = {FilterArguments("-crop", {1000, 1000})};  // NOLINT
    controller = FilterController(arguments);

    controller.ApplyFilters(result_file.GetImage());

    crop = CropFilter(1000, 1000);  // NOLINT
    crop.Apply(expected_file.GetImage());

    REQUIRE(expected_file == result_file);

    expected_file = origin;
    result_file = origin;
    arguments = {FilterArguments("-crop", {1000, 1000}), FilterArguments("-gs", {})};  // NOLINT
    controller = FilterController(arguments);

    controller.ApplyFilters(result_file.GetImage());

    crop = CropFilter(1000, 1000);  // NOLINT
    crop.Apply(expected_file.GetImage());
    gs.Apply(expected_file.GetImage());

    REQUIRE(expected_file == result_file);

    expected_file = origin;
    result_file = origin;
    arguments = {FilterArguments("-crop", {1000, 1000}), FilterArguments("-gs", {}),  // NOLINT
                 FilterArguments("-edge", {-0.73})};                                  // NOLINT
    controller = FilterController(arguments);

    controller.ApplyFilters(result_file.GetImage());

    crop = CropFilter(1000, 1000);      // NOLINT
    edge = EdgeDetectionFilter(-0.75);  // NOLINT
    crop.Apply(expected_file.GetImage());
    gs.Apply(expected_file.GetImage());
    edge.Apply(expected_file.GetImage());

    REQUIRE(expected_file == result_file);

    expected_file = origin;
    result_file = origin;
    arguments = {FilterArguments("-crop", {1000, 1000}), FilterArguments("-gs", {}),  // NOLINT
                 FilterArguments("-edge", {-2314.73})};                               // NOLINT
    controller = FilterController(arguments);

    controller.ApplyFilters(result_file.GetImage());

    crop = CropFilter(1000, 1000);         // NOLINT
    edge = EdgeDetectionFilter(-2314.75);  // NOLINT
    crop.Apply(expected_file.GetImage());
    gs.Apply(expected_file.GetImage());
    edge.Apply(expected_file.GetImage());

    REQUIRE(expected_file == result_file);

    expected_file = origin;
    result_file = origin;
    arguments = {FilterArguments("-crop", {1000, 1000}), FilterArguments("-gs", {}),  // NOLINT
                 FilterArguments("-edge", {2314.73})};                                // NOLINT
    controller = FilterController(arguments);

    controller.ApplyFilters(result_file.GetImage());

    crop = CropFilter(1000, 1000);        // NOLINT
    edge = EdgeDetectionFilter(2314.75);  // NOLINT
    crop.Apply(expected_file.GetImage());
    gs.Apply(expected_file.GetImage());
    edge.Apply(expected_file.GetImage());

    REQUIRE(expected_file == result_file);

    expected_file = origin;
    result_file = origin;
    arguments = {FilterArguments("-crop", {1000.12, 1000.73}), FilterArguments("-gs", {})};  // NOLINT
    controller = FilterController(arguments);

    controller.ApplyFilters(result_file.GetImage());

    crop = CropFilter(1000, 1000);  // NOLINT
    crop.Apply(expected_file.GetImage());
    gs.Apply(expected_file.GetImage());

    REQUIRE(expected_file == result_file);

    expected_file = origin;
    result_file = origin;
    arguments = {FilterArguments("-neg", {}), FilterArguments("-gs", {})};
    controller = FilterController(arguments);

    controller.ApplyFilters(result_file.GetImage());

    neg.Apply(expected_file.GetImage());
    gs.Apply(expected_file.GetImage());

    REQUIRE(expected_file == result_file);

    expected_file = origin;
    result_file = origin;
    arguments = {FilterArguments("-neg", {}), FilterArguments("-gs", {}), FilterArguments("-neg", {}),
                 FilterArguments("-gs", {})};
    controller = FilterController(arguments);

    controller.ApplyFilters(result_file.GetImage());

    neg.Apply(expected_file.GetImage());
    gs.Apply(expected_file.GetImage());
    neg.Apply(expected_file.GetImage());
    gs.Apply(expected_file.GetImage());

    REQUIRE(expected_file == result_file);

    expected_file = origin;
    result_file = origin;
    arguments = {FilterArguments("-sharp", {}), FilterArguments("-sharp", {}), FilterArguments("-neg", {})};
    controller = FilterController(arguments);

    controller.ApplyFilters(result_file.GetImage());

    sharp.Apply(expected_file.GetImage());
    sharp.Apply(expected_file.GetImage());
    neg.Apply(expected_file.GetImage());

    REQUIRE(expected_file == result_file);

    expected_file = origin;
    result_file = origin;
    arguments = {FilterArguments("-sharp", {}), FilterArguments("-sharp", {}), FilterArguments("-neg", {}),
                 FilterArguments("-blur", {7.5}), FilterArguments("-oct", {11})};  // NOLINT
    controller = FilterController(arguments);
    blur = GaussianBlurFilter(7.5);  // NOLINT
    oct = OctogonalizeFilter(11);    // NOLINT

    controller.ApplyFilters(result_file.GetImage());

    sharp.Apply(expected_file.GetImage());
    sharp.Apply(expected_file.GetImage());
    neg.Apply(expected_file.GetImage());
    blur.Apply(expected_file.GetImage());
    oct.Apply(expected_file.GetImage());

    REQUIRE(expected_file == result_file);
}

TEST_CASE("Unsupported filter") {
    std::string input_path =
        "../test_script/data/lenna.bmp";
    BMP result_file = BMPHandler::ReadFromFile(input_path);
    std::vector<FilterArguments> arguments = {FilterArguments("-croap", {1000, 1000})};  // NOLINT
    FilterController controller(arguments);

    REQUIRE_THROWS_AS(controller.ApplyFilters(result_file.GetImage()),
                      filter_controller_exceptions::UnsupportedFilterException);

    arguments = {FilterArguments("-crop", {1000, 1000}), FilterArguments("-oct", {13}),  // NOLINT
                 FilterArguments("-1892132", {1231341, 123124, 1241.2})};                // NOLINT
    controller = FilterController(arguments);

    REQUIRE_THROWS_AS(controller.ApplyFilters(result_file.GetImage()),
                      filter_controller_exceptions::UnsupportedFilterException);

    arguments = {FilterArguments("jhftcyrtd", {1000, 1000})};  // NOLINT
    controller = FilterController(arguments);

    REQUIRE_THROWS_AS(controller.ApplyFilters(result_file.GetImage()),
                      filter_controller_exceptions::UnsupportedFilterException);

    arguments = {FilterArguments("896324HAKJSG&*^!*@#&", {1000, 1000})};  // NOLINT
    controller = FilterController(arguments);

    REQUIRE_THROWS_AS(controller.ApplyFilters(result_file.GetImage()),
                      filter_controller_exceptions::UnsupportedFilterException);

    arguments = {FilterArguments("", {1000, 1000})};  // NOLINT
    controller = FilterController(arguments);

    REQUIRE_THROWS_AS(controller.ApplyFilters(result_file.GetImage()),
                      filter_controller_exceptions::UnsupportedFilterException);
}

TEST_CASE("Invalid argument number") {
    std::string input_path =
        "/home/user/ami-236-1-Ivan-Khmura-Vaneyk1973/tasks/image_processor/test_script/data/lenna.bmp";
    BMP result_file = BMPHandler::ReadFromFile(input_path);
    std::vector<FilterArguments> arguments = {FilterArguments("-crop", {1000, 1000, 123})};  // NOLINT
    FilterController controller(arguments);

    REQUIRE_THROWS_AS(controller.ApplyFilters(result_file.GetImage()),
                      filter_controller_exceptions::InvalidArgumentNumberException);

    arguments = {FilterArguments("-crop", {1000, 1000}), FilterArguments("-oct", {13}),  // NOLINT
                 FilterArguments("-gs", {1231341, 123124, 1241.2})};                     // NOLINT
    controller = FilterController(arguments);

    REQUIRE_THROWS_AS(controller.ApplyFilters(result_file.GetImage()),
                      filter_controller_exceptions::InvalidArgumentNumberException);

    arguments = {FilterArguments("-gs", {12.12})};  // NOLINT
    controller = FilterController(arguments);

    REQUIRE_THROWS_AS(controller.ApplyFilters(result_file.GetImage()),
                      filter_controller_exceptions::InvalidArgumentNumberException);

    arguments = {FilterArguments("-neg", {12.12})};  // NOLINT
    controller = FilterController(arguments);

    REQUIRE_THROWS_AS(controller.ApplyFilters(result_file.GetImage()),
                      filter_controller_exceptions::InvalidArgumentNumberException);

    arguments = {FilterArguments("-neg", {}), FilterArguments("-crop", {12})};  // NOLINT
    controller = FilterController(arguments);

    REQUIRE_THROWS_AS(controller.ApplyFilters(result_file.GetImage()),
                      filter_controller_exceptions::InvalidArgumentNumberException);

    arguments = {FilterArguments("-neg", {}), FilterArguments("-crop", {})};
    controller = FilterController(arguments);

    REQUIRE_THROWS_AS(controller.ApplyFilters(result_file.GetImage()),
                      filter_controller_exceptions::InvalidArgumentNumberException);

    arguments = {FilterArguments("-neg", {}), FilterArguments("-crop", {12, 12}),  // NOLINT
                 FilterArguments("-oct", {})};                                     // NOLINT
    controller = FilterController(arguments);

    REQUIRE_THROWS_AS(controller.ApplyFilters(result_file.GetImage()),
                      filter_controller_exceptions::InvalidArgumentNumberException);
}

TEST_CASE("Illigal argument") {
    std::string input_path =
        "/home/user/ami-236-1-Ivan-Khmura-Vaneyk1973/tasks/image_processor/test_script/data/lenna.bmp";
    BMP result_file = BMPHandler::ReadFromFile(input_path);
    std::vector<FilterArguments> arguments = {FilterArguments("-crop", {1000, -1000})};  // NOLINT
    FilterController controller(arguments);

    REQUIRE_THROWS_AS(controller.ApplyFilters(result_file.GetImage()),
                      filter_controller_exceptions::IllegalArgumentException);

    arguments = {FilterArguments("-oct", {-123})};  // NOLINT
    controller = FilterController(arguments);

    REQUIRE_THROWS_AS(controller.ApplyFilters(result_file.GetImage()),
                      filter_controller_exceptions::IllegalArgumentException);

    arguments = {FilterArguments("-blur", {-123})};  // NOLINT
    controller = FilterController(arguments);

    REQUIRE_THROWS_AS(controller.ApplyFilters(result_file.GetImage()),
                      filter_controller_exceptions::IllegalArgumentException);

    arguments = {FilterArguments("-blur", {0})};  // NOLINT
    controller = FilterController(arguments);

    REQUIRE_THROWS_AS(controller.ApplyFilters(result_file.GetImage()),
                      filter_controller_exceptions::IllegalArgumentException);

    arguments = {FilterArguments("-crop", {-123, -123})};  // NOLINT
    controller = FilterController(arguments);

    REQUIRE_THROWS_AS(controller.ApplyFilters(result_file.GetImage()),
                      filter_controller_exceptions::IllegalArgumentException);
}
