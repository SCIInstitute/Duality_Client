#pragma once

#include "Scene/DataProvider.h"
#include "Scene/InputVariable.h"
#include "Scene/Communication.h"

#include <memory>
#include <vector>

class SCIRunProvider : public DataProvider {
public:
    SCIRunProvider(const std::string& sceneName, const std::string& fileName, std::vector<InputVariableFloat> floatVariables,
                   std::vector<InputVariableEnum> enumVariables, std::shared_ptr<LazyRpcClient> rpc);

    // DataProvider interface
    std::shared_ptr<std::vector<uint8_t>> fetch() override;
    std::vector<InputVariableFloat*> inputVariablesFloat() override;
    std::vector<InputVariableEnum*> inputVariablesEnum() override;

    std::string fileName() const;

private:
    std::string m_sceneName;
    std::string m_fileName;
    std::vector<InputVariableFloat> m_floatVariables;
    std::vector<InputVariableEnum> m_enumVariables;
    std::shared_ptr<LazyRpcClient> m_rpc;
    bool m_dirty;
};