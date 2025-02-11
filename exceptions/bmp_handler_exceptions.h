#pragma once
#include <stdexcept>

namespace filter_controller_exceptions {

class BadFileException final : public std::invalid_argument {
private:
    static std::string MakeMessage(char mode);

public:
    explicit BadFileException(char mode);
};

class CorruptedFileException final : public std::invalid_argument {
private:
    static std::string MakeMessage();

public:
    explicit CorruptedFileException();
};

class CorruptedFileHeaderException final : public std::invalid_argument {
private:
    static std::string MakeMessage(int field, unsigned int value);

public:
    explicit CorruptedFileHeaderException(int field, unsigned int value);
};

class CorruptedDIBHeaderException final : public std::invalid_argument {
private:
    static std::string MakeMessage();

public:
    explicit CorruptedDIBHeaderException();
};

class UnsupportedDIBHeaderTypeException final : public std::invalid_argument {
private:
    static std::string MakeMessage();

public:
    explicit UnsupportedDIBHeaderTypeException();
};

class UnsupportedDIBHeaderValueException final : public std::invalid_argument {
private:
    static std::string MakeMessage(unsigned int field, unsigned int value, unsigned int excpected);

public:
    explicit UnsupportedDIBHeaderValueException(unsigned int field, unsigned int value, unsigned int excpected);
};

}  // namespace filter_controller_exceptions