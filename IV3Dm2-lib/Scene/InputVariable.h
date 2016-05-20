#pragma once

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
    using ValueType = float;

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
    using ValueType = std::string;
    
    InputVariableEnum(const Info& info);
    Info info() const;
    void setValue(std::string value);
    std::string value() const;

private:
    Info m_info;
    std::string m_value;
};