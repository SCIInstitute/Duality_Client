#pragma once

#include <map>
#include <string>
#include <vector>

struct FloatVariableInfo {
    std::string name;
    int index;
    float lowerBound;
    float upperBound;
    float stepSize;
    float defaultValue;
};

struct EnumVariableInfo {
    std::string name;
    int index;
    std::vector<std::string> values;
    std::string defaultValue;
};

struct VariableInfos {
    std::vector<FloatVariableInfo> floatInfos;
    std::vector<EnumVariableInfo> enumInfos;
};

using VariableInfoMap = std::map<std::string, VariableInfos>;
