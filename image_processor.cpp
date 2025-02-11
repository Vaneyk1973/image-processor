#include "utils/argument_parser.h"
#include "filters/filter_controller.h"
#include "io_handlers/bmp_handler.h"
#include "utils/bmp.h"

#include <iostream>

void PrintHelp() {
    std::cout
        << "To apply filters to the image, provide arguments to the program in the following format.\n"
           "./image_processor {input file path} {output file path} -filter1_name [filter1_argument1, "
           "filter2_argument2, ...] -filter2_name [filter2_argument1, filter2_argument2, ...] ...\n"
           "Currently supported filters:\n"
           "-crop _width _height | _width, _height - unsigned integer, max value - 4 294 967 295 | Crops the image, "
           "the resulting image will be _width pixel width and _height pixels height, starting from top left corner\n"
           "-gs | Transforms colors to grayscale\n"
           "-neg | Negates colors\n"
           "-shrp | Sharpening filter\n"
           "-edge _treshold | _treshold - real number, max absolute value - 1.7e308 | Edge detection, _treshold - "
           "sensitivity\n"
           "-blur _sigma | _sigma - positive real number, max value - 1.7e308 | Applies gaussian blur with parameter "
           "_sigma to the image. Caution, high values of _sigma may cause the program to work unexpectedly long\n"
           "-oct _size | _size - unsigned odd integer - 4 294 967 295 | Fills the picture with an octagon pattern\n";
}

int main(int argc, char** argv) {
    if (argc == 1) {
        PrintHelp();
        return 0;
    }
    std::string input_path;
    std::string output_path;
    std::vector<FilterArguments> args;
    const std::vector<std::string> command_line_arguments(argv, argv + argc);
    try {
        ArgumentParser::ParseArguments(command_line_arguments, input_path, output_path, args);
    } catch (const std::exception& e) {
        std::cout << e.what();
        return 1;
    }
    try {
        BMP bmp = BMPHandler::ReadFromFile(input_path);
        FilterController controller(args);
        controller.ApplyFilters(bmp.GetImage());
        BMPHandler::WriteToFile(output_path, bmp);
    } catch (const std::exception& e) {
        std::cout << e.what();
    }
    return 0;
}
