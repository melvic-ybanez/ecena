//
// Created by Melvic Ybanez on 12/29/22.
//

#ifndef ECENA_UTILS_H
#define ECENA_UTILS_H

#include <sstream>

namespace rt {
    template<typename T>
    std::string join_with(const std::vector<T> &elems, const std::string &separator) {
        std::stringstream str;
        size_t i = 0;
        for (auto &elem: elems) {
            str << elem;
            i++;
            if (i != elems.size()) str << separator;
        }

        return str.str();
    }

    template<typename T>
    std::string join(const std::vector<T> &elems) {
        return join_with(elems, ", ");
    }

    template<typename T>
    std::string join_array(const std::vector<T> &elems) {
        return "[ " + join_with(elems, ", ") + " ]";
    }

    template<typename T>
    std::string join_object(const std::vector<T> &elems) {
        return "{ " + join_with(elems, ", ") + " }";
    }
}

#endif //ECENA_UTILS_H
