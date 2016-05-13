#pragma once

#include "Scene/DataProvider.h"
#include "Scene/ParameterManipulator.h"
#include "Scene/ServerAdapter.h"

#include <memory>
#include <vector>

class SCIRunProvider : public DataProvider, public ParameterManipulator {
public:
    SCIRunProvider(const ServerAdapter* server, std::string network, std::vector<InputParameterFloat> floatParameters,
                   std::vector<InputParameterEnum> enumParameters);

    // DataProvider interface
    void accept(DataProviderDispatcher& dispatcher) override;

    // ParameterManipulator interface
    std::string name() const override;
    std::vector<InputParameterFloat> floatParameters() const override;
    std::vector<InputParameterEnum> enumParameters() const override;
    void setFloatValue(const std::string& name, float value) override;
    std::map<std::string, float> floatValues() const override;
    
    std::shared_ptr<std::vector<uint8_t>> generate() const;
    bool isDirty() const;
    void setDirty(bool dirty);
    
private:
    const ServerAdapter* m_server;
    std::string m_network;
    std::vector<InputParameterFloat> m_floatParameters;
    std::vector<InputParameterEnum> m_enumParameters;
    std::map<std::string, float> m_floatValues;
    bool m_dirty;
};