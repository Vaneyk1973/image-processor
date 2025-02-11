#include "bmp_handler.h"

#include "../exceptions/bmp_handler_exceptions.h"
#include "../utils/bmp.h"

#include <cmath>
#include <fstream>

namespace {

constexpr unsigned int MaxColor = 255;
constexpr unsigned int MaxByte = 256;

unsigned int BytesToInt(const char* bytes, const unsigned int offset, const unsigned int size) {
    unsigned int power = 1;
    unsigned int result = 0;
    for (unsigned int i = 0; i < size; ++i) {
        result += static_cast<unsigned int>(static_cast<unsigned char>(bytes[offset + i])) * power;
        power *= MaxByte;
    }
    return result;
}

std::vector<char> IntToBytes(unsigned int value, const unsigned int size) {
    std::vector<char> bytes(size);
    std::size_t i = 0;
    while (value > 0) {
        bytes[i] = static_cast<char>(value % MaxByte);
        value /= MaxByte;
        ++i;
    }
    while (i < size) {
        bytes[i] = 0;
        ++i;
    }
    return bytes;
}

Pixel BytesToPixel(const char* bytes, const unsigned int offset) {
    const double blue = static_cast<double>(static_cast<unsigned char>(bytes[offset])) / MaxColor;
    const double green = static_cast<double>(static_cast<unsigned char>(bytes[offset + 1])) / MaxColor;
    const double red = static_cast<double>(static_cast<unsigned char>(bytes[offset + 2])) / MaxColor;
    return Pixel(red, green, blue);
}

void PixelToBytes(const Pixel& pixel, const unsigned int size, std::vector<char>& bytes) {
    const std::array<double, 3> colors = pixel.GetColors();
    for (unsigned int i = 1; i <= size; ++i) {
        bytes.push_back(static_cast<char>(static_cast<unsigned int>(std::round(colors[size - i] * MaxColor))));
    }
}

}  // namespace

void BMPHandler::BMPFileHeaderHandler::ReadHeaderFromFile(std::ifstream& file, std::vector<unsigned int>& args) {
    std::vector<char> file_header(FileHeaderSize);
    unsigned int typical_value = 0;
    file.read(file_header.data(), FileHeaderSize);
    if (file.fail()) {
        throw filter_controller_exceptions::CorruptedFileException();
    }
    typical_value = BytesToInt(file_header.data(), IdOffset, IdL);
    if (typical_value != Id) {
        throw filter_controller_exceptions::CorruptedFileHeaderException(0, typical_value);
    }
    args.push_back(BytesToInt(file_header.data(), FullSizeOffset, FullSizeL));
    args.push_back(BytesToInt(file_header.data(), BitmapPointerOffset, BitmapPointerL));
}

void BMPHandler::BMPFileHeaderHandler::WriteHeaderToFile(std::ofstream& file, const unsigned int full_size) {
    file.write(IntToBytes(Id, IdL).data(), IdL);
    file.write(IntToBytes(full_size, FullSizeL).data(), FullSizeL);
    file.write(IntToBytes(ApplicationSpecificFields, ApplicationSpecificFieldsL * 2).data(),
               ApplicationSpecificFieldsL * 2);
    file.write(IntToBytes(BitmapPointer, BitmapPointerL).data(), BitmapPointerL);
}

void BMPHandler::BMPBitmapDIBHeaderHandler::ReadDIBHeaderFromFile(std::ifstream& file,
                                                                  std::vector<unsigned int>& args) {
    unsigned int typical_value = 0;
    std::vector<char> file_info_header(DIBHeaderSizeL);
    file.read(file_info_header.data(), DIBHeaderSizeL);
    if (file.fail()) {
        throw filter_controller_exceptions::CorruptedFileException();
    }
    typical_value = BytesToInt(file_info_header.data(), DIBHeaderSizeOffset, DIBHeaderSizeL);
    if (typical_value != DIBHeaderSize) {
        throw filter_controller_exceptions::UnsupportedDIBHeaderTypeException();
    }
    for (unsigned int i = 0; i < DIBHeaderSizeL; ++i) {
        file.unget();
    }
    file_info_header.clear();
    file_info_header.resize(DIBHeaderSize);
    file.read(file_info_header.data(), DIBHeaderSize);
    args.push_back(BytesToInt(file_info_header.data(), WidthOffset, WidthL));
    args.push_back(BytesToInt(file_info_header.data(), HeightOffset, HeightL));
    typical_value = BytesToInt(file_info_header.data(), ColorPlanesNumberOffset, ColorPlanesNumberL);
    if (typical_value != ColorPlanesNumber) {
        throw filter_controller_exceptions::UnsupportedDIBHeaderValueException(0, typical_value, ColorPlanesNumber);
    }
    typical_value = BytesToInt(file_info_header.data(), BitsPerPixelOffset, BitsPerPixelL);
    if (typical_value != BMP::BitsPerPixel) {
        throw filter_controller_exceptions::UnsupportedDIBHeaderValueException(1, typical_value, BMP::BitsPerPixel);
    }
    typical_value = BytesToInt(file_info_header.data(), CompressionMethodOffset, CompressionMethodL);
    if (typical_value != CompressionMethod) {
        throw filter_controller_exceptions::UnsupportedDIBHeaderValueException(2, typical_value, CompressionMethod);
    }
    args.push_back(BytesToInt(file_info_header.data(), RawBitmapSizeOffset, RawBitmapSizeL));
    typical_value = BytesToInt(file_info_header.data(), ColorsNumberOffset, ColorsNumberL);
    if (typical_value != ColorsNumber) {
        throw filter_controller_exceptions::UnsupportedDIBHeaderValueException(3, typical_value, ColorsNumber);
    }
    typical_value = BytesToInt(file_info_header.data(), ImportantColorsNumberOffset, ImportantColorsNumberL);
    if (typical_value != ImportantColorsNumber) {
        throw filter_controller_exceptions::UnsupportedDIBHeaderValueException(4, typical_value, ImportantColorsNumber);
    }
}

void BMPHandler::BMPBitmapDIBHeaderHandler::WriteDIBHeaderToFile(std::ofstream& file,
                                                                 const std::vector<unsigned int>& args) {
    file.write(IntToBytes(DIBHeaderSize, DIBHeaderSizeL).data(), DIBHeaderSizeL);
    file.write(IntToBytes(args[0], WidthL).data(), WidthL);
    file.write(IntToBytes(args[1], HeightL).data(), HeightL);
    file.write(IntToBytes(ColorPlanesNumber, ColorPlanesNumberL).data(), ColorPlanesNumberL);
    file.write(IntToBytes(BMP::BitsPerPixel, BitsPerPixelL).data(), BitsPerPixelL);
    file.write(IntToBytes(CompressionMethod, CompressionMethodL).data(), CompressionMethodL);
    file.write(IntToBytes(args[2], RawBitmapSizeL).data(), RawBitmapSizeL);
    file.write(IntToBytes(Hresolution, HresolutionL).data(), HresolutionL);
    file.write(IntToBytes(Vresolution, VresolutionL).data(), VresolutionL);
    file.write(IntToBytes(ColorsNumber, ColorsNumberL).data(), ColorsNumberL);
    file.write(IntToBytes(ImportantColorsNumber, ImportantColorsNumberL).data(), ImportantColorsNumberL);
}

BMP BMPHandler::ReadFromFile(const std::string& path) {
    std::ifstream file(path, std::ios_base::binary);
    if (file.fail()) {
        throw filter_controller_exceptions::BadFileException('r');
    }
    std::vector<unsigned int> file_header_args;
    std::vector<unsigned int> file_dib_header_args;
    BMPFileHeaderHandler::ReadHeaderFromFile(file, file_header_args);
    const unsigned int file_size = file_header_args[0];
    const unsigned int bitmap_pointer = file_header_args[1];
    BMPBitmapDIBHeaderHandler::ReadDIBHeaderFromFile(file, file_dib_header_args);
    if (bitmap_pointer != BitmapPointer) {
        throw filter_controller_exceptions::CorruptedFileHeaderException(2, bitmap_pointer);
    }
    const unsigned int image_width = file_dib_header_args[0];
    const unsigned int image_height = file_dib_header_args[1];
    unsigned int bitmap_size = file_dib_header_args[2];
    if (bitmap_size == 0) {
        bitmap_size = CalculateBitmapDataSize(image_width, image_height, BMP::BytesPerPixel);
    }
    if (CalculateBitmapDataSize(image_width, image_height, BMP::BytesPerPixel) != bitmap_size) {
        throw filter_controller_exceptions::CorruptedDIBHeaderException();
    }
    std::vector<char> bitmap(bitmap_size);
    Image image(image_height, image_width);
    file.read(bitmap.data(), bitmap_size);
    if (file.fail()) {
        throw filter_controller_exceptions::CorruptedFileException();
    } else if (file_size != bitmap_size + DIBHeaderSize + FileHeaderSize) {
        throw filter_controller_exceptions::CorruptedFileHeaderException(1, file_size);
    }
    unsigned int pixel_offset = 0;
    unsigned int line_offset = (4 - image_width * BMP::BytesPerPixel % 4) % 4;
    for (unsigned int line = 1; line <= image_height; ++line) {
        for (unsigned int pixel = 0; pixel < image_width; ++pixel) {
            image.GetPixel(image_height - line, pixel) = BytesToPixel(bitmap.data(), pixel_offset);
            pixel_offset += BMP::BytesPerPixel;
        }
        pixel_offset += line_offset;
    }
    file.close();
    return BMP(image);
}

void BMPHandler::WriteToFile(const std::string& path, BMP& bmp) {
    std::ofstream file(path, std::ios_base::binary);
    if (file.fail()) {
        throw filter_controller_exceptions::BadFileException('w');
    }
    const unsigned int bmp_size = bmp.Size() + FileHeaderSize + DIBHeaderSize;
    BMPFileHeaderHandler::WriteHeaderToFile(file, bmp_size);
    const std::vector<unsigned int> file_dib_header_args{bmp.GetImage().GetWidth(), bmp.GetImage().GetHeight(),
                                                         bmp.Size()};
    BMPBitmapDIBHeaderHandler::WriteDIBHeaderToFile(file, file_dib_header_args);
    if (file.fail()) {
        throw filter_controller_exceptions::BadFileException('w');
    }
    const unsigned int line_offset = (4 - bmp.GetImage().GetWidth() * BMP::BytesPerPixel % 4) % 4;
    const unsigned int bytes_per_line = bmp.GetImage().GetWidth() * BMP::BytesPerPixel + line_offset;
    for (unsigned int line = 1; line <= bmp.GetImage().GetHeight(); ++line) {
        std::vector<char> line_bytes;
        for (unsigned int pixel = 0; pixel < bmp.GetImage().GetWidth(); ++pixel) {
            PixelToBytes(bmp.GetImage().GetPixel(bmp.GetImage().GetHeight() - line, pixel), BMP::BytesPerPixel,
                         line_bytes);
        }
        for (unsigned int i = 0; i < line_offset; ++i) {
            line_bytes.push_back(0);
        }
        file.write(line_bytes.data(), bytes_per_line);
        if (file.fail()) {
            throw filter_controller_exceptions::BadFileException('w');
        }
    }
    file.close();
}
