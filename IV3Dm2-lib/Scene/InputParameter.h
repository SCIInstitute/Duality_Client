#pragma once

#include <string>
#include <vector>

struct InputParameterFloat {
    std::string name;
    float lowerBound;
    float upperBound;
    float stepSize;
    float defaultValue;
};

struct InputParameterEnum {
    std::string name;
    std::vector<std::string> values;
    std::string defaultValue;
};