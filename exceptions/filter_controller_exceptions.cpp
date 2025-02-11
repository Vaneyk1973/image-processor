#include "filter_controller_exceptions.h"

std::string filter_controller_exceptions::UnsupportedFilterException::MakeMessage(const std::string& filter) {
    return "Filter " + filter +
           " is not supported. For the list of supported filters call the programm without arguments.";
}

filter_controller_exceptions::UnsupportedFilterException::UnsupportedFilterException(const std::string& filter)
    : std::invalid_argument(MakeMessage(filter)) {
}

std::string filter_controller_exceptions::InvalidArgumentNumberException::MakeMessage(const std::string& filter,
                                                                                      unsigned int arguments_number) {
    return "Passed " + std::to_string(arguments_number) + " for the filter " + filter + ". This filter does not take " +
           std::to_string(arguments_number) + " arguments. For the arguments the filter " + filter +
           " takes call the programm without arguments.";
}

filter_controller_exceptions::InvalidArgumentNumberException::InvalidArgumentNumberException(
    const std::string& filter, const unsigned int arguments_number)
    : std::invalid_argument(MakeMessage(filter, arguments_number)) {
}

std::string filter_controller_exceptions::IllegalArgumentException::MakeMessage(const std::string& filter) {
    return "Some of the arguments passed to the " + filter +
           " filter are in the wrong format. For more information about supported filters and their arguments call "
           "programm without arguments.";
}

filter_controller_exceptions::IllegalArgumentException::IllegalArgumentException(const std::string& filter)
    : std::invalid_argument(MakeMessage(filter)) {
}