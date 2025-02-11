cmake_minimum_required(VERSION 3.8)

project(image-processor)

set(CMAKE_MODULE_PATH              "${CMAKE_SOURCE_DIR}/cmake")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
set(CMAKE_EXPORT_COMPILE_COMMANDS  ON)

include(cmake/BuildFlags.cmake)
include(cmake/TestSolution.cmake)

find_package(Catch REQUIRED)

include_directories(util)

add_catch(test_argument_parser
        utils/argument_parser.cpp
        exceptions/argument_parser_exceptions.cpp
        tests/argument_parser_tests.cpp)

add_catch(test_filter_controller
        tests/filter_controller_tests.cpp
        io_handlers/bmp_handler.cpp
        filters/filter_controller.cpp
        filters/filters.cpp
        filters/base_filters.cpp
        utils/bmp.cpp
        utils/argument_parser.cpp
        exceptions/argument_parser_exceptions.cpp
        exceptions/filter_controller_exceptions.cpp
        exceptions/bmp_handler_exceptions.cpp
)

add_executable(
        image_processor
        image_processor.cpp
        io_handlers/bmp_handler.cpp
        io_handlers/bmp_handler.h
        filters/filters.cpp
        filters/filters.h
        utils/argument_parser.cpp
        utils/argument_parser.h
        exceptions/argument_parser_exceptions.cpp
        exceptions/argument_parser_exceptions.h
        exceptions/bmp_handler_exceptions.cpp
        exceptions/bmp_handler_exceptions.h
        utils/bmp.cpp
        utils/bmp.h
        filters/base_filters.cpp
        filters/base_filters.h
        filters/filter_controller.cpp
        filters/filter_controller.h
        exceptions/filter_controller_exceptions.cpp
        exceptions/filter_controller_exceptions.h
)
