cmake_minimum_required(VERSION 3.14)

project(image-processor)

set(CMAKE_MODULE_PATH              "${CMAKE_SOURCE_DIR}/cmake")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
set(CMAKE_EXPORT_COMPILE_COMMANDS  ON)

include(cmake/BuildFlags.cmake)
include(cmake/AddCatch.cmake)

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
        filters/filters.cpp
        utils/argument_parser.cpp
        exceptions/argument_parser_exceptions.cpp
        exceptions/bmp_handler_exceptions.cpp
        utils/bmp.cpp
        filters/base_filters.cpp
        filters/filter_controller.cpp
        exceptions/filter_controller_exceptions.cpp
)
