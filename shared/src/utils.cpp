//
// Created by Melvic Ybanez on 2/5/23.
//

#include "../include/utils.h"

namespace rt {
    bool starts_with(const std::string &str, const std::string &search_str) {
        return str.rfind(search_str, 0) == 0;
    }

    void replace_all(std::string &str, const std::string &from, const std::string &to) {
        auto pos = str.find(from);
        while (pos != std::string::npos) {
            str.replace(pos, from.length(), to);
            pos = str.find(from);
        }
    }
}