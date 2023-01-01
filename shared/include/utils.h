//
// Created by Melvic Ybanez on 12/29/22.
//

#ifndef ECENA_UTILS_H
#define ECENA_UTILS_H

#include <sstream>
#include <optional>

namespace rt {
    template<typename T, typename Fn>
    std::string join_with_helper(const std::vector<T> &elems, const std::string &separator, Fn f) {
        std::stringstream str;
        size_t i = 0;
        for (auto &elem: elems) {
            f(str, elem);
            i++;
            if (i != elems.size()) str << separator;
        }

        return str.str();
    }

    template<typename T>
    std::string join_with(const std::vector<T> &elems, const std::string &separator) {
        return join_with_helper(elems, separator, [](auto &str, auto &elem) { str << elem; });
    }

    template<typename T>
    std::string join_with(const std::vector<std::unique_ptr<T>> &elems, const std::string &separator) {
        return join_with_helper(elems, separator, [](auto &str, auto &elem) { str << *elem; });
    }

    template<typename T>
    std::string join(const std::vector<T> &elems) {
        return join_with(elems, ", ");
    }

    template<typename T>
    std::string join_to_array(const std::vector<T> &elems) {
        return "[ " + join(elems) + " ]";
    }

    template<typename T>
    std::string join_to_object(const std::vector<T> &elems) {
        return "{ " + join(elems) + " }";
    }

    template<typename T>
    std::string optional_to_str(const std::optional<T> &optional) {
        std::stringstream ss;

        if (optional.has_value()) ss << optional.value();
        else ss << "null";

        return ss.str();
    }
}

#endif //ECENA_UTILS_H
