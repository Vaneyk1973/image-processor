#pragma once
#include <stdexcept>

namespace argument_parser_exceptions {

class BadFileFormatException final : public std::invalid_argument {
private:
    static std::string MakeMessage(const std::string& parameter);

public:
    explicit BadFileFormatException(const std::string&);
};

class NotEnoughArgumentsException final : public std::invalid_argument {
private:
    static std::string MakeMessage(const std::string& arguments_num);

public:
    explicit NotEnoughArgumentsException(const std::string& arguments_num);
};

class BadFilterNameException final : public std::invalid_argument {
private:
    static std::string MakeMessage(const std::string& name);

public:
    explicit BadFilterNameException(const std::string& name);
};

class BadFilterParameterException final : public std::invalid_argument {
private:
    static std::string MakeMessage(const std::string& parameter);

public:
    explicit BadFilterParameterException(const std::string& parameter);
};

class TooLargeFilterParameterException final : public std::invalid_argument {
private:
    static std::string MakeMessage(const std::string& parameter);

public:
    explicit TooLargeFilterParameterException(const std::string& parameter);
};

}  // namespace argument_parser_exceptions
