#pragma once

#include "../utils/argument_parser.h"
#include "../utils/bmp.h"

#include <map>

class FilterController {
private:
    std::vector<FilterArguments> filters_;

    static const std::map<std::string, std::size_t> AVAILIABLE_FILTERS_ARGUMENTS_NUM;

public:
    explicit FilterController(const std::vector<FilterArguments>& filters);

    void ApplyFilters(Image& img);
};