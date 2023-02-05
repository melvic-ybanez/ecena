//
// Created by Melvic Ybanez on 2/5/23.
//

#include "../include/utils.h"

namespace rt {
    bool starts_with(const std::string &str, const std::string &search_str) {
        return str.rfind(search_str, 0) == 0;
    }
}