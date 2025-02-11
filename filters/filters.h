#pragma once

#include "../utils/bmp.h"
#include "base_filters.h"

class CropFilter final : public Filter {
private:
    unsigned int new_width_ = 0;
    unsigned int new_height_ = 0;

public:
    CropFilter(unsigned int width, unsigned int height);

    void Apply(Image& img) override;
};

class GrayscaleFilter final : public Filter {
private:
    static constexpr double RedToGs = 0.299;
    static constexpr double GreenToGs = 0.587;
    static constexpr double BlueToGs = 0.114;

public:
    void Apply(Image& img) override;
};

class NegativeFilter final : public Filter {
public:
    void Apply(Image& img) override;
};

class GaussianBlurFilter final : public MatrixFilter {
private:
    double sigma_ = 1;

    double GaussianFunction(int x0, int x) const;

    std::vector<std::vector<double>> x_matrix_;

    std::vector<std::vector<double>> y_matrix_;

    void ApplyX(Image& img);

    void ApplyY(Image& img);

public:
    explicit GaussianBlurFilter(double sigma);

    void Apply(Image& img) override;
};

class SharpeningFilter final : public MatrixFilter {
public:
    SharpeningFilter();
};

class EdgeDetectionFilter final : public MatrixFilter {
private:
    double treshold_ = 0;

public:
    explicit EdgeDetectionFilter(double treshold);

    void Apply(Image& img) override;
};

class OctogonalizeFilter final : public Filter {
private:
    int octagon_size_ = 0;

    std::vector<std::vector<double>> octagon_matrix_;

    std::vector<std::vector<double>> square_matrix_;

    void FillOctagonMatrix();

    void FillSquareMatrix();

    static Pixel CalculateMedian(Image& img, int line, int pixel, int size);

public:
    explicit OctogonalizeFilter(int octagon_size);

    void Apply(Image& img) override;
};
