#pragma once

#include "Scene/InputVariable.h"

#include <vector>
#include <string>

class InputVariableFloat {
public:
    struct Info {
        std::string name;
        int index;
        float lowerBound;
        float upperBound;
        float stepSize;
        float defaultValue;
    };

    InputVariableFloat(const Info& info);
    Info info() const;
    void setValue(float value);
    float value() const;

private:
    Info m_info;
    float m_value;
};

class InputVariableEnum {
public:
    struct Info {
        std::string name;
        int index;
        std::vector<std::string> values;
        std::string defaultValue;
    };

    InputVariableEnum(const Info& info);
    Info info() const;
    void setValue(std::string value);
    std::string value() const;

private:
    Info m_info;
    std::string m_value;
};