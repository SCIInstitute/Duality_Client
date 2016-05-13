#pragma once

#include "Scene/DataProvider.h"
#include "Scene/InputParameter.h"

class SCIRunProvider : public DataProvider {
public:
    SCIRunProvider(std::string network, std::vector<InputParameterFloat> floatParameters, std::vector<InputParameterEnum> enumParameters);

    // DataProvider interface
    void accept(DataProviderDispatcher& dispatcher) override;

    std::string network() const;
    std::vector<InputParameterFloat> floatParameters() const;
    std::vector<InputParameterEnum> enumParameters() const;

private:
    std::string m_network;
    std::vector<InputParameterFloat> m_floatParameters;
    std::vector<InputParameterEnum> m_enumParameters;
};