#include "duality/InputVariable.h"

#include "duality/DownloadProvider.h"
#include "duality/PythonProvider.h"

InputVariableFloat::InputVariableFloat(const Info& info)
    : m_info(info)
    , m_value(info.defaultValue) {}

InputVariableFloat::Info InputVariableFloat::info() const {
    return m_info;
}

void InputVariableFloat::setValue(float value) {
    m_value = value;
}

float InputVariableFloat::value() const {
    return m_value;
}


InputVariableEnum::InputVariableEnum(const Info& info)
    : m_info(info)
    , m_value(info.defaultValue) {}

InputVariableEnum::Info InputVariableEnum::info() const {
    return m_info;
}

void InputVariableEnum::setValue(std::string value) {
    m_value = std::move(value);
}

std::string InputVariableEnum::value() const {
    return m_value;
}
