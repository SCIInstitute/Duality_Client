#include "Scene/ParameterManipulator.h"

#include "Scene/DownloadProvider.h"
#include "Scene/SCIRunProvider.h"

ParameterManipulatorFloat::ParameterManipulatorFloat(InputParameterFloat parameter, SCIRunProvider* provider)
    : m_parameter(std::move(parameter))
    , m_provider(provider) {}

InputParameterFloat ParameterManipulatorFloat::parameter() const {
    return m_parameter;
}

void ParameterManipulatorFloat::setValue(float value) {
    m_provider->setFloatValue(m_parameter.name, value);
}

float ParameterManipulatorFloat::getValue() const {
    return m_provider->getFloatValue(m_parameter.name);
}

void ParameterManipulatorCollector::dispatch(DownloadProvider& provider) {
    // nothing to do
}

void ParameterManipulatorCollector::dispatch(SCIRunProvider& provider) {
    auto floatParams = provider.floatParameters();
    for (const auto& param : floatParams) {
        m_floatManipulators.push_back(ParameterManipulatorFloat(param, &provider));
    }
    auto enumParams = provider.enumParameters();
    for (const auto& param : enumParams) {
        m_enumManipulators.push_back(ParameterManipulatorEnum(param, &provider));
    }
}

std::vector<ParameterManipulatorFloat> ParameterManipulatorCollector::floatManipulators() const {
    return m_floatManipulators;
}

std::vector<ParameterManipulatorEnum> ParameterManipulatorCollector::enumManipulators() const {
    return m_enumManipulators;
}

ParameterManipulatorEnum::ParameterManipulatorEnum(InputParameterEnum parameter, SCIRunProvider* provider)
    : m_parameter(std::move(parameter))
    , m_provider(provider) {}

InputParameterEnum ParameterManipulatorEnum::parameter() const {
    return m_parameter;
}

void ParameterManipulatorEnum::setValue(std::string value) {
    m_provider->setEnumValue(m_parameter.name, value);
}

std::string ParameterManipulatorEnum::getValue() const {
    return m_provider->getEnumValue(m_parameter.name);
}
