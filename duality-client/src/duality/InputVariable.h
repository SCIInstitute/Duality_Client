#pragma once

#include "duality/VariableInfo.h"

#include <vector>
#include <string>

class InputVariableFloat {
public:
    using ValueType = float;

    InputVariableFloat(const FloatVariableInfo& info);
    FloatVariableInfo info() const;
    void setValue(float value);
    float value() const;

private:
    FloatVariableInfo m_info;
    float m_value;
};

class InputVariableEnum {
public:
    using ValueType = std::string;
    
    InputVariableEnum(const EnumVariableInfo& info);
    EnumVariableInfo info() const;
    void setValue(std::string value);
    std::string value() const;

private:
    EnumVariableInfo m_info;
    std::string m_value;
};