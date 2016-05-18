#pragma once

#include "Scene/DataProvider.h"
#include "Scene/ParameterManipulator.h"
#include "Scene/ServerAdapter.h"

#include <memory>
#include <vector>

class SCIRunProvider : public DataProvider {
public:
    SCIRunProvider(const ServerAdapter* server, std::string network, std::vector<InputParameterFloat> floatParameters,
                   std::vector<InputParameterEnum> enumParameters);

    // DataProvider interface
    void accept(DataProviderDispatcher& dispatcher) override;
    std::shared_ptr<std::vector<uint8_t>> fetch() override;

    std::string network() const;
    std::vector<InputParameterFloat> floatParameters() const;
    std::vector<InputParameterEnum> enumParameters() const;
    void setFloatValue(const std::string& name, float value);
    float getFloatValue(const std::string& name) const;
    void setEnumValue(const std::string& name, const std::string& value);
    std::string getEnumValue(const std::string& name) const;

private:
    const ServerAdapter* m_server;
    std::string m_network;
    std::vector<InputParameterFloat> m_floatParameters;
    std::vector<InputParameterEnum> m_enumParameters;
    std::map<std::string, float> m_floatValues;
    std::map<std::string, std::string> m_enumValues;
    bool m_dirty;
};