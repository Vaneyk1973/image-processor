#include "bmp.h"

#include <cmath>

Pixel::Pixel() : colors_({0, 0, 0}) {
}

Pixel::Pixel(const double red, const double green, const double blue) : colors_({red, green, blue}) {
}

Pixel::Pixel(const std::array<double, 3>& colors) : colors_(colors) {
}

std::array<double, 3> Pixel::GetColors() const {
    return colors_;
}

void Pixel::Normalize() {
    for (int i = 0; i < 3; ++i) {
        colors_[i] = std::max(0.0, std::min(1.0, colors_[i]));
    }
}

Pixel Pixel::operator+(const Pixel& rhs) const {
    std::array<double, 3> result;
    for (int i = 0; i < 3; ++i) {
        result[i] = colors_[i] + rhs.colors_[i];
    }
    return Pixel(result);
}

Pixel Pixel::operator-(const Pixel& rhs) const {
    std::array<double, 3> result;
    for (int i = 0; i < 3; ++i) {
        result[i] = colors_[i] - rhs.colors_[i];
    }
    return Pixel(result);
}

Pixel Pixel::operator*(const double rhs) const {
    std::array<double, 3> result;
    for (int i = 0; i < 3; ++i) {
        result[i] = colors_[i] * rhs;
    }
    return Pixel(result);
}

Pixel Pixel::operator+=(const Pixel& rhs) {
    for (int i = 0; i < 3; ++i) {
        colors_[i] = colors_[i] + rhs.colors_[i];
    }
    return *this;
}

Pixel Pixel::operator/=(const double x) {
    for (int i = 0; i < 3; ++i) {
        colors_[i] = colors_[i] / x;
    }
    return *this;
}

Pixel Pixel::operator*=(double x) {
    for (int i = 0; i < 3; ++i) {
        colors_[i] = colors_[i] * x;
    }
    return *this;
}

bool Pixel::operator==(const Pixel& rhs) const {
    return rhs.colors_ == colors_;
}

Image::Image() {
}

Image::Image(const unsigned int height, const unsigned int width)
    : width_(width), height_(height), data_(std::vector(height, std::vector<Pixel>(width))) {
}

unsigned int Image::GetHeight() const {
    return height_;
}

void Image::SetHeight(const unsigned int new_height) {
    height_ = new_height;
}
void Image::SetWidth(const unsigned int new_width) {
    width_ = new_width;
}

bool Image::operator==(const Image& rhs) const {
    return height_ == rhs.height_ && width_ == rhs.width_ && data_ == rhs.data_;
}

unsigned int Image::GetWidth() const {
    return width_;
}

Pixel& Image::GetPixel(const unsigned int height, const unsigned int width) {
    return data_[height][width];
}

Pixel Image::GetPixel(const unsigned int height, const unsigned int width) const {
    return data_[height][width];
}

std::vector<std::vector<Pixel>>& Image::GetData() {
    return data_;
}

BMP::BMP() : size_(0), line_offset_(0), image_(Image()) {
}

BMP::BMP(const Image& image) : image_(image) {
    line_offset_ = image.GetWidth() * BytesPerPixel % 4;
    size_ = CalculateBitmapDataSize(image.GetWidth(), image.GetHeight(), BytesPerPixel);
}

Image& BMP::GetImage() {
    return image_;
}

unsigned int BMP::Size() const {
    return size_;
}

bool BMP::operator==(const BMP& rhs) const {
    return size_ == rhs.size_ && line_offset_ == rhs.line_offset_ && image_ == rhs.image_;
}

unsigned int CalculateBitmapDataSize(const unsigned int image_width, const unsigned int image_height,
                                     const unsigned int bytes_per_pixel) {
    return (image_width * bytes_per_pixel + (4 - image_width * bytes_per_pixel % 4) % 4) * image_height;
}
