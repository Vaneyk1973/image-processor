#include "argument_parser_exceptions.h"

std::string argument_parser_exceptions::NotEnoughArgumentsException::MakeMessage(const std::string& arguments_num) {
    return "Not enough arguments provided. Excpected >=3, got " + arguments_num + '.';
}
argument_parser_exceptions::NotEnoughArgumentsException::NotEnoughArgumentsException(const std::string& arguments_num)
    : std::invalid_argument(MakeMessage(arguments_num)) {
}

std::string argument_parser_exceptions::BadFilterNameException::MakeMessage(const std::string& name) {
    return "Bad filter name passed. Expected -filtername, got \"" + name + "\".";
}

argument_parser_exceptions::BadFilterNameException::BadFilterNameException(const std::string& name)
    : std::invalid_argument(MakeMessage(name)) {
}

std::string argument_parser_exceptions::BadFilterParameterException::MakeMessage(const std::string& parameter) {
    return "Bad filter parameter passed. Expected real number, got \"" + parameter + "\".";
}

argument_parser_exceptions::BadFilterParameterException::BadFilterParameterException(const std::string& parameter)
    : std::invalid_argument(MakeMessage(parameter)) {
}

std::string argument_parser_exceptions::TooLargeFilterParameterException::MakeMessage(const std::string& parameter) {
    return "Passed parameter value is too large. Expected real number no more than 1.7e308 and no less than 1.7e-308 "
           "by absolute value, got \"" +
           parameter + "\".";
}

argument_parser_exceptions::TooLargeFilterParameterException::TooLargeFilterParameterException(
    const std::string& parameter)
    : std::invalid_argument(MakeMessage(parameter)) {
}

std::string argument_parser_exceptions::BadFileFormatException::MakeMessage(const std::string& parameter) {
    return "Bad file format. Expected a .bmp file, got \"" + parameter + "\".";
}

argument_parser_exceptions::BadFileFormatException::BadFileFormatException(const std::string& parameter)
    : std::invalid_argument(MakeMessage(parameter)) {
}
