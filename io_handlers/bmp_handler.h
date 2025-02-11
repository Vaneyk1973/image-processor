#pragma once

#include "../utils/bmp.h"

#include <string>

class BMPHandler {
private:
    class BMPFileHeaderHandler {
    private:
        static constexpr unsigned int IdOffset = 0;
        static constexpr unsigned int FullSizeOffset = 2;
        static constexpr unsigned int BitmapPointerOffset = 10;

        static constexpr unsigned int IdL = 2;
        static constexpr unsigned int FullSizeL = 4;
        static constexpr unsigned int ApplicationSpecificFieldsL = 2;
        static constexpr unsigned int BitmapPointerL = 4;

        // typical values
        static constexpr unsigned int Id = 0x4D42;
        static constexpr unsigned int ApplicationSpecificFields = 0;

    public:
        static void ReadHeaderFromFile(std::ifstream& file, std::vector<unsigned int>& args);

        static void WriteHeaderToFile(std::ofstream& file, const unsigned int full_size);
    };

    class BMPBitmapDIBHeaderHandler {
    private:
        static constexpr unsigned int DIBHeaderSizeOffset = 0;
        static constexpr unsigned int WidthOffset = 4;
        static constexpr unsigned int HeightOffset = 8;
        static constexpr unsigned int ColorPlanesNumberOffset = 12;
        static constexpr unsigned int BitsPerPixelOffset = 14;
        static constexpr unsigned int CompressionMethodOffset = 16;
        static constexpr unsigned int RawBitmapSizeOffset = 20;
        static constexpr unsigned int HresolutionOffset = 24;
        static constexpr unsigned int VresolutionOffset = 28;
        static constexpr unsigned int ColorsNumberOffset = 32;
        static constexpr unsigned int ImportantColorsNumberOffset = 36;

        static constexpr unsigned int DIBHeaderSizeL = 4;
        static constexpr unsigned int WidthL = 4;
        static constexpr unsigned int HeightL = 4;
        static constexpr unsigned int ColorPlanesNumberL = 2;
        static constexpr unsigned int BitsPerPixelL = 2;
        static constexpr unsigned int CompressionMethodL = 4;
        static constexpr unsigned int RawBitmapSizeL = 4;
        static constexpr unsigned int HresolutionL = 4;
        static constexpr unsigned int VresolutionL = 4;
        static constexpr unsigned int ColorsNumberL = 4;
        static constexpr unsigned int ImportantColorsNumberL = 4;

        // typical values
        static constexpr unsigned int ColorPlanesNumber = 1;
        static constexpr unsigned int CompressionMethod = 0;
        static constexpr unsigned int Hresolution = 0xB13;
        static constexpr unsigned int Vresolution = 0xB13;
        static constexpr unsigned int ColorsNumber = 0;
        static constexpr unsigned int ImportantColorsNumber = 0;

    public:
        static void ReadDIBHeaderFromFile(std::ifstream& file, std::vector<unsigned int>& args);

        static void WriteDIBHeaderToFile(std::ofstream& file, const std::vector<unsigned int>& args);
    };

    static constexpr unsigned int FileHeaderSize = 14;
    static constexpr unsigned int DIBHeaderSize = 40;
    static constexpr unsigned int BitmapPointer = 54;

public:
    static BMP ReadFromFile(const std::string& path);

    static void WriteToFile(const std::string& path, BMP& bmp);
};
