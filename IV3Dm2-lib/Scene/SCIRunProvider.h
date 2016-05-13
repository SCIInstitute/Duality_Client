#pragma once

#include "Scene/DataProvider.h"
#include "Scene/ParameterManipulator.h"

class SCIRunProvider : public DataProvider, public ParameterManipulator {
public:
    SCIRunProvider(std::string network, std::vector<InputParameterFloat> floatParameters, std::vector<InputParameterEnum> enumParameters);

    // DataProvider interface
    void accept(DataProviderDispatcher& dispatcher) override;

    // ParameterManipulator interface
    std::string name() const override;
    std::vector<InputParameterFloat> floatParameters() const override;
    std::vector<InputParameterEnum> enumParameters() const override;

private:
    std::string m_network;
    std::vector<InputParameterFloat> m_floatParameters;
    std::vector<InputParameterEnum> m_enumParameters;
};