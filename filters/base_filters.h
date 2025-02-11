#pragma once

#include "../utils/bmp.h"

class FilterMatrix {
public:
    static const std::vector<std::vector<double>> SHARPENING;
    static const std::vector<std::vector<double>> EDGE_DETECTION;
};

class Filter {
public:
    virtual void Apply(Image& img) = 0;

    virtual ~Filter() = default;
};

class MatrixFilter : public Filter {
protected:
    std::vector<std::vector<double>> filter_matrix_;

    void ApplyMatrix(Image& img) const;

public:
    explicit MatrixFilter(const std::vector<std::vector<double>>& filter_matrix);

    void Apply(Image& img) override;
};
