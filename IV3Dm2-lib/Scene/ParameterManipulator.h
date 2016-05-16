#pragma once

#include "Scene/InputParameter.h"
#include "Scene/DataProviderDispatcher.h"

#include <string>
#include <map>

class ParameterManipulatorFloat {
public:
    ParameterManipulatorFloat(InputParameterFloat parameter, SCIRunProvider* provider);
    InputParameterFloat parameter() const;
    void setValue(float value);
    float getValue() const;
private:
    InputParameterFloat m_parameter;
    SCIRunProvider* m_provider;
};

class ParameterManipulatorEnum {
public:
    ParameterManipulatorEnum(InputParameterEnum parameter, SCIRunProvider* provider);
    InputParameterEnum parameter() const;
    void setValue(std::string value);
    std::string getValue() const;
private:
    InputParameterEnum m_parameter;
    SCIRunProvider* m_provider;
};

class ParameterManipulatorCollector : public DataProviderDispatcher {
public:
    void dispatch(DownloadProvider& provider) override;
    void dispatch(SCIRunProvider& provider) override;

    std::vector<ParameterManipulatorFloat> floatManipulators() const;
    std::vector<ParameterManipulatorEnum> enumManipulators() const;

private:
    std::vector<ParameterManipulatorFloat> m_floatManipulators;
    std::vector<ParameterManipulatorEnum> m_enumManipulators;
};