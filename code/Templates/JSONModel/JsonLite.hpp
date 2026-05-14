#pragma once
#include <string>
#include <vector>
#include <stdexcept>
#include <cctype>

namespace JsonLite {

// Trim whitespace
inline void trim(std::string& s) {
    while(!s.empty() && std::isspace(s.back())) s.pop_back();
    size_t i = 0;
    while(i < s.size() && std::isspace(s[i])) i++;
    s.erase(0, i);
}

// Extract value for "key": ...
inline std::string extract(const std::string& s, const std::string& key) {
    auto pos = s.find("\"" + key + "\"");
    if(pos == std::string::npos)
        throw std::runtime_error("Key not found: " + key);

    pos = s.find(":", pos);
    if(pos == std::string::npos)
        throw std::runtime_error("Malformed JSON");

    pos++; // move past colon

    // Skip whitespace
    while(pos < s.size() && std::isspace(s[pos])) pos++;

    // Extract until comma or end brace
    size_t end = pos;
    int bracket = 0;

    if(s[pos] == '[') {
        bracket = 1;
        end++;
        while(end < s.size() && bracket > 0) {
            if(s[end] == '[') bracket++;
            if(s[end] == ']') bracket--;
            end++;
        }
    } else {
        while(end < s.size() && s[end] != ',' && s[end] != '}')
            end++;
    }

    return s.substr(pos, end - pos);
}

inline double to_double(const std::string& s) {
    return std::stod(s);
}

inline int to_int(const std::string& s) {
    return std::stoi(s);
}

inline std::vector<double> to_vec(const std::string& s) {
    std::vector<double> v;
    size_t i = 0;

    while (i < s.size()) {
        // Skip until digit, minus, or dot
        while (i < s.size() &&
               !((s[i] >= '0' && s[i] <= '9') || s[i] == '-' || s[i] == '.'))
            i++;

        if (i >= s.size()) break;

        // Parse number
        size_t end = i;
        while (end < s.size() &&
               ((s[end] >= '0' && s[end] <= '9') || s[end] == '-' || s[end] == '.' || s[end] == 'e' || s[end] == 'E'))
            end++;

        v.push_back(std::stod(s.substr(i, end - i)));
        i = end;
    }

    return v;
}


} // namespace JsonLite

