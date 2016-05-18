#pragma once

#include "Scene/DataProvider.h"
#include "Scene/InputVariable.h"
#include "Scene/ServerAdapter.h"

#include <memory>
#include <vector>

class SCIRunProvider : public DataProvider {
public:
    SCIRunProvider(const ServerAdapter* server, std::string network, std::vector<InputVariableFloat> floatVariables,
        std::vector<InputVariableEnum> enumVariables);

    // DataProvider interface
    std::shared_ptr<std::vector<uint8_t>> fetch() override;
    std::vector<InputVariableFloat*> inputVariablesFloat() override;
    std::vector<InputVariableEnum*> inputVariablesEnum() override;

    std::string network() const;

private:
    const ServerAdapter* m_server;
    std::string m_network;
    std::vector<InputVariableFloat> m_floatVariables;
    std::vector<InputVariableEnum> m_enumVariables;
    bool m_dirty;
};