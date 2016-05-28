#include "src/duality/InputVariable.h"

#include "src/duality/DownloadProvider.h"
#include "src/duality/PythonProvider.h"

InputVariableFloat::InputVariableFloat(const FloatVariableInfo& info)
    : m_info(info)
    , m_value(info.defaultValue) {}

FloatVariableInfo InputVariableFloat::info() const {
    return m_info;
}

void InputVariableFloat::setValue(float value) {
    m_value = value;
}

float InputVariableFloat::value() const {
    return m_value;
}


InputVariableEnum::InputVariableEnum(const EnumVariableInfo& info)
    : m_info(info)
    , m_value(info.defaultValue) {}

EnumVariableInfo InputVariableEnum::info() const {
    return m_info;
}

void InputVariableEnum::setValue(std::string value) {
    m_value = std::move(value);
}

std::string InputVariableEnum::value() const {
    return m_value;
}
