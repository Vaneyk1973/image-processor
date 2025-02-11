#pragma once
#include <stdexcept>

namespace filter_controller_exceptions {

class UnsupportedFilterException final : public std::invalid_argument {
private:
    static std::string MakeMessage(const std::string& filter);

public:
    explicit UnsupportedFilterException(const std::string& filter);
};

class InvalidArgumentNumberException final : public std::invalid_argument {
private:
    static std::string MakeMessage(const std::string& filter, unsigned int arguments_number);

public:
    explicit InvalidArgumentNumberException(const std::string& filter, unsigned int arguments_number);
};

class IllegalArgumentException final : public std::invalid_argument {
private:
    static std::string MakeMessage(const std::string& filter);

public:
    explicit IllegalArgumentException(const std::string& filter);
};

}  // namespace filter_controller_exceptions
