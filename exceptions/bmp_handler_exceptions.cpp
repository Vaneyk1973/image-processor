#include "bmp_handler_exceptions.h"

std::string filter_controller_exceptions::BadFileException::MakeMessage(const char mode) {
    if (mode == 'r') {
        return "Couldn't open a file. Check whether you have the rights to accsess the file and the file exists.";
    } else if (mode == 'w') {
        return "Couldn't create or write to a file. Check whether you have the rights to accsess the file.";
    } else {
        return "Couldn't open a file.";
    }
}

filter_controller_exceptions::BadFileException::BadFileException(const char mode)
    : std::invalid_argument(MakeMessage(mode)) {
}

std::string filter_controller_exceptions::CorruptedFileException::MakeMessage() {
    return "Couldn't read from a given input file. The file might be corrupted.";
}

filter_controller_exceptions::CorruptedFileException::CorruptedFileException() : std::invalid_argument(MakeMessage()) {
}

std::string filter_controller_exceptions::CorruptedFileHeaderException::MakeMessage(const int field,
                                                                                    const unsigned int value) {
    if (field == 0) {
        return "The ID field of a file header in the given file has bad value. Expected 1978, got \"" +
               std::to_string(value) + "\"";
    } else if (field == 1) {
        return "The value in the field of a file header for file size is not equal to the file size.";
    } else if (field == 2) {
        return "The value in the field of a file header for offset to the bitmap data a does not point to the bitmap "
               "data in the given file.";
    } else {
        return "";
    }
}

filter_controller_exceptions::CorruptedFileHeaderException::CorruptedFileHeaderException(const int field,
                                                                                         const unsigned int value)
    : std::invalid_argument(MakeMessage(field, value)) {
}

std::string filter_controller_exceptions::CorruptedDIBHeaderException::MakeMessage() {
    return "The DIB header in a give file might be corrupted. Values in width and height fields do not correlate with "
           "a value in bitmap size filed";
}

filter_controller_exceptions::CorruptedDIBHeaderException::CorruptedDIBHeaderException()
    : std::invalid_argument(MakeMessage()) {
}

std::string filter_controller_exceptions::UnsupportedDIBHeaderTypeException::MakeMessage() {
    return "The type of DIB header in this file is not supported";
}

filter_controller_exceptions::UnsupportedDIBHeaderTypeException::UnsupportedDIBHeaderTypeException()
    : std::invalid_argument(MakeMessage()) {
}

std::string filter_controller_exceptions::UnsupportedDIBHeaderValueException::MakeMessage(unsigned int field,
                                                                                          unsigned int value,
                                                                                          unsigned int excpected) {
    if (field == 0) {
        return "Unsupported number of color planes in the file. Expected " + std::to_string(excpected) + ", got " +
               std::to_string(value) + ".";
    } else if (field == 1) {
        return "Unsupported number of bits per pixel in the file. Expected " + std::to_string(excpected) + ", got " +
               std::to_string(value) + ".";
    }
    if (field == 2) {
        return "Unsupported compression method in the file. Expected " + std::to_string(excpected) + ", got " +
               std::to_string(value) + ".";
    }
    if (field == 3) {
        return "Unsupported number of colors in the file. Expected " + std::to_string(excpected) + ", got " +
               std::to_string(value) + ".";
    }
    if (field == 4) {
        return "Unsupported number of important colors in the file. Expected " + std::to_string(excpected) + ", got " +
               std::to_string(value) + ".";
    } else {
        return "";
    }
}

filter_controller_exceptions::UnsupportedDIBHeaderValueException::UnsupportedDIBHeaderValueException(
    const unsigned int field, const unsigned int value, unsigned int excpected)
    : std::invalid_argument(MakeMessage(field, value, excpected)) {
}