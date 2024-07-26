#pragma once

#include <string>
#include <regex>
#include <vector>

#include <iostream>

std::vector<std::string> split(const std::string &st, const std::regex &re) {
    std::vector<std::string> result;
    std::sregex_token_iterator iter(st.begin(), st.end(), re, -1);
    std::sregex_token_iterator end;
    while (iter != end) {
        result.push_back(*iter++);
    }
    return result;
}

bool endsWith(std::string st, std::string suffix) {
    if (st.size() < suffix.size()) {
        return false;
    }
    return st.compare(st.size() - suffix.size(), suffix.size(), suffix) == 0;
}

bool startsWith(std::string st, std::string prefix) {
    if (st.size() < prefix.size()) {
        return false;
    }
    return st.compare(0, prefix.size(), prefix) == 0;
}

double UnSimpFrac(std::string st) {
    std::regex re("\\/");
    std::vector<std::string> parts = split(st, re);
    if (parts.size() == 2) {
        return std::stod(parts[0]) / std::stod(parts[1]);
    }
    return std::stod(st);
}

std::string trim(const std::string& str) {
    size_t first = str.find_first_not_of(' ');
    if (std::string::npos == first) {
        return str;
    }
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}

std::string repeat(const std::string& str, int n) {
    std::string result;
    for (int i = 0; i < n; i++) {
        result += str;
    }
    return result;
}

std::string repeat(const char c, int n) {
    return repeat(std::string(1, c), n);
}

std::string join(const std::vector<std::string>& elements, const std::string& delimiter) {
    std::ostringstream os;
    for (size_t i = 0; i < elements.size(); ++i) {
        os << elements[i];
        if (i != elements.size() - 1) {
            os << delimiter;
        }
    }
    return os.str();
}

uint32_t getUnicodeCodepointAtIndex(const std::string& str, size_t index) {
    size_t i = 0;
    size_t strLength = str.size();

    while (i < strLength) {
        uint32_t codepoint = 0;
        unsigned char byte = str[i];

        if ((byte & 0x80) == 0x00) {
            // 1-byte sequence (ASCII)
            codepoint = byte;
            ++i;
        } else if ((byte & 0xE0) == 0xC0) {
            // 2-byte sequence
            if (i + 1 >= strLength) throw std::out_of_range("Index out of range");
            codepoint = ((byte & 0x1F) << 6) | (str[i + 1] & 0x3F);
            i += 2;
        } else if ((byte & 0xF0) == 0xE0) {
            // 3-byte sequence
            if (i + 2 >= strLength) throw std::out_of_range("Index out of range");
            codepoint = ((byte & 0x0F) << 12) | ((str[i + 1] & 0x3F) << 6) | (str[i + 2] & 0x3F);
            i += 3;
        } else if ((byte & 0xF8) == 0xF0) {
            // 4-byte sequence
            if (i + 3 >= strLength) throw std::out_of_range("Index out of range");
            codepoint = ((byte & 0x07) << 18) | ((str[i + 1] & 0x3F) << 12) | ((str[i + 2] & 0x3F) << 6) | (str[i + 3] & 0x3F);
            i += 4;
        } else {
            throw std::runtime_error("Invalid UTF-8 encoding");
        }

        if (i > index) {
            return codepoint;
        }
    }

    throw std::out_of_range("Index out of range");
}
