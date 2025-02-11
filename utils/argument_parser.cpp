#include "argument_parser.h"

#include "../exceptions/argument_parser_exceptions.h"

FilterArguments::FilterArguments() {
}

FilterArguments::FilterArguments(const std::string& filter_name, const std::vector<double>& filter_arguments)
    : filter_name_(filter_name), filter_arguments_(filter_arguments) {
}

std::string FilterArguments::GetFilterName() const {
    return filter_name_;
}

std::vector<double> FilterArguments::GetFilterArguments() const {
    return filter_arguments_;
}

double FilterArguments::At(const size_t index) const {
    return filter_arguments_[index];
}

std::size_t FilterArguments::ArgumentsNum() const {
    return filter_arguments_.size();
}

bool FilterArguments::operator==(const FilterArguments& rhs) const {
    return filter_name_ == rhs.filter_name_ && filter_arguments_ == rhs.filter_arguments_;
}

void ArgumentParser::ParseArguments(const std::vector<std::string>& arguments, std::string& input_path,
                                    std::string& output_path, std::vector<FilterArguments>& filters) {
    const size_t arguments_num = arguments.size();
    if (arguments_num < 3) {
        throw argument_parser_exceptions::NotEnoughArgumentsException(std::to_string(arguments_num));
    }
    if (!arguments[1].ends_with(".bmp")) {
        throw argument_parser_exceptions::BadFileFormatException(arguments[1]);
    }
    if (!arguments[2].ends_with(".bmp")) {
        throw argument_parser_exceptions::BadFileFormatException(arguments[1]);
    }
    input_path = arguments[1];
    output_path = arguments[2];
    size_t i = 3;
    while (i < arguments_num) {
        if (arguments[i][0] != '-') {
            throw argument_parser_exceptions::BadFilterNameException(arguments[i]);
        } else if (arguments[i].length() == 1) {
            throw argument_parser_exceptions::BadFilterNameException(arguments[i]);
        } else if (!isalpha(arguments[i][1])) {
            throw argument_parser_exceptions::BadFilterNameException(arguments[i]);
        } else {
            for (size_t j = 1; j < arguments[i].size(); ++j) {
                if (!isalnum(arguments[i][j])) {
                    throw argument_parser_exceptions::BadFilterNameException(arguments[i]);
                }
            }
        }
        std::string filter_name = arguments[i];
        std::vector<double> filter_arguments(0);
        ++i;
        while (i < arguments_num && (arguments[i][0] != '-' ||
                                     (arguments[i].size() > 1 && arguments[i][0] == '-' && isdigit(arguments[i][1])))) {
            size_t last = 0;
            double parameter = 0;
            try {
                parameter = std::stod(arguments[i], &last);
            } catch (const std::out_of_range&) {
                throw argument_parser_exceptions::TooLargeFilterParameterException(arguments[i]);
            } catch (const std::exception&) {
                throw argument_parser_exceptions::BadFilterParameterException(arguments[i]);
            }
            if (last != arguments[i].size()) {
                throw argument_parser_exceptions::BadFilterParameterException(arguments[i]);
            }
            filter_arguments.push_back(parameter);
            ++i;
        }
        filters.emplace_back(filter_name, filter_arguments);
    }
}
