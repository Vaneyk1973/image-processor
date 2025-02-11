#include "filters.h"

#include <cmath>
#include <numbers>
#include <set>

CropFilter::CropFilter(const unsigned int width, const unsigned int height) : new_width_(width), new_height_(height) {
}

void CropFilter::Apply(Image& img) {
    img.GetData().resize(std::min(img.GetHeight(), new_height_));
    for (size_t line = 0; line < std::min(img.GetHeight(), new_height_); ++line) {
        img.GetData()[line].resize(std::min(img.GetHeight(), new_width_));
    }
    img.SetHeight(std::min(img.GetHeight(), new_height_));
    img.SetWidth(std::min(img.GetWidth(), new_width_));
}

void GrayscaleFilter::Apply(Image& img) {
    for (unsigned int line = 0; line < img.GetHeight(); ++line) {
        for (unsigned int pixel = 0; pixel < img.GetWidth(); ++pixel) {
            std::array<double, 3> old_colors = img.GetPixel(line, pixel).GetColors();
            double new_color = old_colors[0] * RedToGs + old_colors[1] * GreenToGs + old_colors[2] * BlueToGs;
            img.GetPixel(line, pixel) = {new_color, new_color, new_color};
        }
    }
}

void NegativeFilter::Apply(Image& img) {
    for (unsigned int line = 0; line < img.GetHeight(); ++line) {
        for (unsigned int pixel = 0; pixel < img.GetWidth(); ++pixel) {
            std::array<double, 3> old_colors = img.GetPixel(line, pixel).GetColors();
            img.GetPixel(line, pixel) = {1 - old_colors[0], 1 - old_colors[1], 1 - old_colors[2]};
        }
    }
}

SharpeningFilter::SharpeningFilter() : MatrixFilter(FilterMatrix::SHARPENING) {
}

EdgeDetectionFilter::EdgeDetectionFilter(double treshold)
    : MatrixFilter(FilterMatrix::EDGE_DETECTION), treshold_(treshold) {
}

void EdgeDetectionFilter::Apply(Image& img) {
    GrayscaleFilter gs;
    gs.Apply(img);
    ApplyMatrix(img);
    for (unsigned int line = 0; line < img.GetHeight(); ++line) {
        for (unsigned int pixel = 0; pixel < img.GetWidth(); ++pixel) {
            if (img.GetPixel(line, pixel).GetColors()[0] > treshold_) {
                img.GetPixel(line, pixel) = {1, 1, 1};
            } else {
                img.GetPixel(line, pixel) = {0, 0, 0};
            }
        }
    }
}

GaussianBlurFilter::GaussianBlurFilter(const double sigma) : MatrixFilter({}), sigma_(sigma) {
    const int half_size = static_cast<int>(std::ceil(2.7 * sigma_));
    const int matrix_size = half_size * 2 + 1;
    x_matrix_ = std::vector(1, std::vector<double>(matrix_size, 0));
    y_matrix_ = std::vector(matrix_size, std::vector<double>(1, 0));
    for (int i = 0; i < matrix_size; ++i) {
        x_matrix_[0][i] = GaussianFunction(half_size, i);
        y_matrix_[i][0] = GaussianFunction(half_size, i);
    }
}

double GaussianBlurFilter::GaussianFunction(const int x0, const int x) const {
    const double exponent = -(x0 - x) * (x0 - x) / (2 * sigma_ * sigma_);
    return std::pow(std::numbers::e, exponent) / (std::sqrt(2 * std::numbers::pi) * sigma_);
}

void GaussianBlurFilter::ApplyX(Image& img) {
    x_matrix_.swap(filter_matrix_);
    ApplyMatrix(img);
    x_matrix_.swap(filter_matrix_);
}

void GaussianBlurFilter::ApplyY(Image& img) {
    y_matrix_.swap(filter_matrix_);
    ApplyMatrix(img);
    y_matrix_.swap(filter_matrix_);
}

void GaussianBlurFilter::Apply(Image& img) {
    ApplyX(img);
    ApplyY(img);
}

void OctogonalizeFilter::FillOctagonMatrix() {
    octagon_matrix_ = std::vector(octagon_size_, std::vector<double>(octagon_size_, 0));
    for (int i = 0; i < octagon_size_ / 2 + 1; ++i) {
        for (int j = 0; j < octagon_size_; ++j) {
            if (std::abs(j - octagon_size_ / 2) <= i + octagon_size_ / 6) {  // NOLINT
                octagon_matrix_[i][j] = 1;
            }
        }
    }
    for (int i = octagon_size_ / 2 + 1; i < octagon_size_; ++i) {
        octagon_matrix_[i] = octagon_matrix_[octagon_size_ - i - 1];
    }
}

void OctogonalizeFilter::FillSquareMatrix() {
    square_matrix_ = std::vector(octagon_size_ - 1, std::vector<double>(octagon_size_ - 1, 0));
    for (int i = 0; i < octagon_size_ / 2; ++i) {
        for (int j = 0; j < octagon_size_ / 2; ++j) {
            square_matrix_[i][j] = 1 - octagon_matrix_[octagon_size_ / 2 + i + 1][octagon_size_ / 2 + j + 1];
        }
    }
    for (int i = octagon_size_ / 2; i < octagon_size_ - 1; ++i) {
        for (int j = 0; j < octagon_size_ / 2; ++j) {
            square_matrix_[i][j] = 1 - octagon_matrix_[i - octagon_size_ / 2][octagon_size_ / 2 + j + 1];
        }
    }
    for (int i = 0; i < octagon_size_ / 2; ++i) {
        for (int j = octagon_size_ / 2; j < octagon_size_ - 1; ++j) {
            square_matrix_[i][j] = 1 - octagon_matrix_[octagon_size_ / 2 + i + 1][j - octagon_size_ / 2];
        }
    }
    for (int i = octagon_size_ / 2; i < octagon_size_ - 1; ++i) {
        for (int j = octagon_size_ / 2; j < octagon_size_ - 1; ++j) {
            square_matrix_[i][j] = 1 - octagon_matrix_[i - octagon_size_ / 2][j - octagon_size_ / 2];
        }
    }
}
Pixel OctogonalizeFilter::CalculateMedian(Image& img, const int line, const int pixel, const int size) {
    const int width = static_cast<int>(img.GetWidth());
    const int height = static_cast<int>(img.GetHeight());
    std::multiset<double> red;
    std::multiset<double> green;
    std::multiset<double> blue;
    for (int i = line; i < std::min(height, line + size); ++i) {
        for (int j = pixel; j < std::min(width, pixel + size); j++) {
            std::array<double, 3> current = img.GetPixel(std::max(0, i), std::max(0, j)).GetColors();
            red.insert(current[0]);
            green.insert(current[1]);
            blue.insert(current[2]);
        }
    }
    return Pixel(*std::next(red.begin(), static_cast<int>(red.size() / 2)),
                 *std::next(green.begin(), static_cast<int>(green.size() / 2)),
                 *std::next(blue.begin(), static_cast<int>(blue.size() / 2)));
}

OctogonalizeFilter::OctogonalizeFilter(const int octagon_size) : octagon_size_(octagon_size) {
    FillOctagonMatrix();
    FillSquareMatrix();
}

void OctogonalizeFilter::Apply(Image& img) {
    Image original = img;
    const int width = static_cast<int>(img.GetWidth());
    const int height = static_cast<int>(img.GetHeight());
    const int square_size = octagon_size_ - 1;
    for (int line = 0; line < height; line += octagon_size_) {
        for (int pixel = 0; pixel < width; pixel += octagon_size_) {
            const Pixel median = CalculateMedian(original, line, pixel, octagon_size_);
            for (int i = line; i < std::min(height, line + octagon_size_); ++i) {
                for (int j = pixel; j < std::min(width, pixel + octagon_size_); j++) {
                    img.GetPixel(i, j) = median * octagon_matrix_[i - line][j - pixel];
                }
            }
        }
    }
    for (int line = -square_size / 2; line < height; line += octagon_size_) {
        for (int pixel = -square_size / 2; pixel < width; pixel += octagon_size_) {
            const Pixel median = CalculateMedian(original, line, pixel, square_size);
            for (int i = line; i < std::min(height, line + square_size); ++i) {
                for (int j = pixel; j < std::min(width, pixel + square_size); j++) {
                    if (i >= 0 && j >= 0) {
                        img.GetPixel(i, j) += median * square_matrix_[i - line][j - pixel];
                    }
                }
            }
        }
    }
}
