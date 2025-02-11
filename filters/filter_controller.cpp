#include "filter_controller.h"

#include "filters.h"
#include "../exceptions/filter_controller_exceptions.h"

const std::map<std::string, std::size_t> FilterController::AVAILIABLE_FILTERS_ARGUMENTS_NUM{
    {"-crop", 2}, {"-gs", 0}, {"-neg", 0}, {"-sharp", 0}, {"-edge", 1}, {"-blur", 1}, {"-oct", 1}};

FilterController::FilterController(const std::vector<FilterArguments>& filters) : filters_(filters) {
}

void FilterController::ApplyFilters(Image& img) {
    for (const FilterArguments& filter : filters_) {
        const std::string filter_name = filter.GetFilterName();
        const std::size_t arg_num = filter.ArgumentsNum();
        if (!AVAILIABLE_FILTERS_ARGUMENTS_NUM.contains(filter_name)) {
            throw filter_controller_exceptions::UnsupportedFilterException(filter_name);
        } else if (arg_num != AVAILIABLE_FILTERS_ARGUMENTS_NUM.at(filter_name)) {
            throw filter_controller_exceptions::InvalidArgumentNumberException(filter_name, arg_num);
        }
        if (filter_name == "-crop") {
            if (filter.At(0) < 0 || filter.At(1) < 0) {
                throw filter_controller_exceptions::IllegalArgumentException(filter_name);
            }
            CropFilter crop(static_cast<unsigned int>(filter.At(0)), static_cast<unsigned int>(filter.At(1)));
            crop.Apply(img);
        } else if (filter_name == "-gs") {
            GrayscaleFilter gs;
            gs.Apply(img);
        } else if (filter_name == "-neg") {
            NegativeFilter neg;
            neg.Apply(img);
        } else if (filter_name == "-sharp") {
            SharpeningFilter sharp;
            sharp.Apply(img);
        } else if (filter_name == "-edge") {
            EdgeDetectionFilter edge(filter.At(0));
            edge.Apply(img);
        } else if (filter_name == "-blur") {
            if (filter.At(0) <= 0) {
                throw filter_controller_exceptions::IllegalArgumentException(filter_name);
            }
            GaussianBlurFilter gaussian(filter.At(0));
            gaussian.Apply(img);
        } else if (filter_name == "-oct") {
            if (filter.At(0) <= 0) {
                throw filter_controller_exceptions::IllegalArgumentException(filter_name);
            }
            OctogonalizeFilter oct(static_cast<int>(filter.At(0)) / 2 * 2 + 1);
            oct.Apply(img);
        } else {
            throw filter_controller_exceptions::UnsupportedFilterException(filter_name);
        }
    }
}
