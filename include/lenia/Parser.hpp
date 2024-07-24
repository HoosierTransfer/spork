#pragma once

#include <vector>
#include <lenia/LeniaTemplate.h>
#include <string>
#include <regex>

LeniaTemplate ParseRule(std::string rule);

std::vector<std::string> split(std::string st, std::regex re);

bool endsWith(std::string st, std::string suffix);

bool startsWith(std::string st, std::string prefix);