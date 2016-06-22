#pragma once

#include "mocca/base/Nullable.h"

#include <map>
#include <string>
#include <vector>

struct FloatVariableInfo {
    int index;
    float lowerBound;
    float upperBound;
    float stepSize;
};

struct EnumVariableInfo {
    int index;
    std::vector<std::string> values;
};

struct FloatVariable {
    std::string name;
    mocca::Nullable<std::string> label;
    FloatVariableInfo info;
    float value;
};

struct EnumVariable {
    std::string name;
    mocca::Nullable<std::string> label;
    EnumVariableInfo info;
    std::string value;
};

struct Variables {
    Variables() = default;
    std::vector<FloatVariable> floatVariables;
    std::vector<EnumVariable> enumVariables;
};

using VariableMap = std::map<std::string, Variables>;