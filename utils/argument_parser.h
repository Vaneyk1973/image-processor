#pragma once

#include <string>
#include <vector>

class FilterArguments {
private:
    std::string filter_name_;
    std::vector<double> filter_arguments_{};

public:
    FilterArguments();

    FilterArguments(const std::string& filter_name, const std::vector<double>& filter_arguments);

    std::string GetFilterName() const;

    std::vector<double> GetFilterArguments() const;

    double At(size_t index) const;

    std::size_t ArgumentsNum() const;

    bool operator==(const FilterArguments& rhs) const;
};

class ArgumentParser {
public:
    static void ParseArguments(const std::vector<std::string>& arguments, std::string& input_path,
                               std::string& output_path, std::vector<FilterArguments>& filters);
};
