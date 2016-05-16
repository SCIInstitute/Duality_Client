#pragma once

#include <string>
#include <vector>

struct InputParameter {
    InputParameter(std::string name_, int index_)
        : name(name_)
        , index(index_) {}
    std::string name;
    int index;
};

struct InputParameterFloat : InputParameter {
    InputParameterFloat(std::string name_, int index_, float lowerBound_, float upperBound_, float stepSize_, float defaultValue_)
        : InputParameter(std::move(name_), index_)
        , lowerBound(lowerBound_)
        , upperBound(upperBound_)
        , stepSize(stepSize_)
        , defaultValue(defaultValue_) {}
    float lowerBound;
    float upperBound;
    float stepSize;
    float defaultValue;
};

struct InputParameterEnum : InputParameter {
    InputParameterEnum(std::string name_, int index_, std::vector<std::string> values_, std::string defaultValue_)
        : InputParameter(std::move(name_), index_)
        , values(std::move(values_))
        , defaultValue(std::move(defaultValue_)) {}
    std::vector<std::string> values;
    std::string defaultValue;
};