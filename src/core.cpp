#include "core.h"

#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <iostream>

namespace {
const std::string CURRENT_VERSION = "1.3.9";
}

const std::string& get_version() {
    return CURRENT_VERSION;
}

void clear_screen() {
#if defined(_WIN32)
    std::system("cls");
#else
    std::cout << "\033[2J\033[H";
#endif
}

std::string lowercase(std::string text) {
    std::transform(text.begin(), text.end(), text.begin(),
        [](unsigned char c) {
            return static_cast<char>(std::tolower(c));
        });
    return text;
}
