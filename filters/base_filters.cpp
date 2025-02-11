#include "base_filters.h"

const std::vector<std::vector<double>> FilterMatrix::SHARPENING{{0, -1, 0}, {-1, 5, -1}, {0, -1, 0}};
const std::vector<std::vector<double>> FilterMatrix::EDGE_DETECTION{{0, -1, 0}, {-1, 4, -1}, {0, -1, 0}};

MatrixFilter::MatrixFilter(const std::vector<std::vector<double>>& filter_matrix) : filter_matrix_(filter_matrix) {
}

void MatrixFilter::ApplyMatrix(Image& img) const {
    Image original = img;
    const int half_matrix_vsize = static_cast<int>(filter_matrix_.size() / 2);
    const int width = static_cast<int>(img.GetWidth());
    const int height = static_cast<int>(img.GetHeight());
    const int half_matrix_hsize = static_cast<int>(filter_matrix_[0].size() / 2);
    for (int line = 0; line < height; ++line) {
        for (int pixel = 0; pixel < width; ++pixel) {
            Pixel wsum;
            for (int dline = -half_matrix_vsize; dline <= half_matrix_vsize; ++dline) {
                for (int dpixel = -half_matrix_hsize; dpixel <= half_matrix_hsize; ++dpixel) {
                    if (line + dline < 0 && pixel + dpixel < 0) {
                        wsum += original.GetPixel(0u, 0u) *
                                filter_matrix_[dline + half_matrix_vsize][dpixel + half_matrix_hsize];
                    } else if (line + dline < 0 && pixel + dpixel >= width) {
                        wsum += original.GetPixel(0u, img.GetWidth() - 1) *
                                filter_matrix_[dline + half_matrix_vsize][dpixel + half_matrix_hsize];
                    } else if (line + dline >= height && pixel + dpixel < 0) {
                        wsum += original.GetPixel(img.GetHeight() - 1, 0u) *
                                filter_matrix_[dline + half_matrix_vsize][dpixel + half_matrix_hsize];
                    } else if (line + dline >= height && pixel + dpixel >= width) {
                        wsum += original.GetPixel(img.GetHeight() - 1, img.GetWidth() - 1) *
                                filter_matrix_[dline + half_matrix_vsize][dpixel + half_matrix_hsize];
                    } else if (line + dline < 0) {
                        wsum += original.GetPixel(0, pixel + dpixel) *
                                filter_matrix_[dline + half_matrix_vsize][dpixel + half_matrix_hsize];
                    } else if (line + dline >= height) {
                        wsum += original.GetPixel(img.GetHeight() - 1, pixel + dpixel) *
                                filter_matrix_[dline + half_matrix_vsize][dpixel + half_matrix_hsize];
                    } else if (pixel + dpixel < 0) {
                        wsum += original.GetPixel(line + dline, 0) *
                                filter_matrix_[dline + half_matrix_vsize][dpixel + half_matrix_hsize];
                    } else if (pixel + dpixel >= width) {
                        wsum += original.GetPixel(line + dline, img.GetWidth() - 1) *
                                filter_matrix_[dline + half_matrix_vsize][dpixel + half_matrix_hsize];
                    } else {
                        wsum += original.GetPixel(line + dline, pixel + dpixel) *
                                filter_matrix_[dline + half_matrix_vsize][dpixel + half_matrix_hsize];
                    }
                }
            }
            wsum.Normalize();
            img.GetPixel(line, pixel) = wsum;
        }
    }
}

void MatrixFilter::Apply(Image& img) {
    ApplyMatrix(img);
}
