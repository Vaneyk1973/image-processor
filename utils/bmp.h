#pragma once

#include <array>
#include <vector>

class Pixel {
private:
    std::array<double, 3> colors_;

public:
    Pixel();

    Pixel(double red, double green, double blue);

    explicit Pixel(const std::array<double, 3>& colors);

    std::array<double, 3> GetColors() const;

    void Normalize();

    Pixel operator+(const Pixel& rhs) const;

    Pixel operator-(const Pixel& rhs) const;

    Pixel operator+=(const Pixel& rhs);

    Pixel operator*(double rhs) const;

    Pixel operator/=(double x);

    Pixel operator*=(double x);

    bool operator==(const Pixel& rhs) const;
};

class Image {
private:
    unsigned int width_ = 0;
    unsigned int height_ = 0;
    std::vector<std::vector<Pixel>> data_ = std::vector<std::vector<Pixel>>(0);

public:
    Image();

    Image(unsigned int height, unsigned int width);

    Pixel GetPixel(unsigned int height, unsigned int width) const;

    Pixel& GetPixel(unsigned int height, unsigned int width);

    std::vector<std::vector<Pixel>>& GetData();

    unsigned int GetWidth() const;

    unsigned int GetHeight() const;

    void SetHeight(unsigned int new_height);

    void SetWidth(unsigned int new_width);

    bool operator==(const Image& rhs) const;
};

class BMP {
private:
    unsigned int size_;
    unsigned int line_offset_;
    Image image_;

public:
    static constexpr unsigned int BitsPerPixel = 24;
    static constexpr unsigned int BytesPerPixel = BitsPerPixel / 8;

    BMP();

    explicit BMP(const Image& image);

    Image& GetImage();

    unsigned int Size() const;

    bool operator==(const BMP& rhs) const;
};

unsigned int CalculateBitmapDataSize(unsigned int image_width, unsigned int image_height, unsigned int bytes_per_pixel);
