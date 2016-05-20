#pragma once

#include "Scene/DataProvider.h"
#include "Scene/InputVariable.h"
#include "Scene/Communication.h"

#include <memory>
#include <vector>

class PythonProvider : public DataProvider {
public:
    PythonProvider(const std::string& sceneName, const std::string& fileName, std::vector<InputVariableFloat> floatVariables,
                   std::vector<InputVariableEnum> enumVariables, std::shared_ptr<LazyRpcClient> rpc);

    // DataProvider interface
    std::shared_ptr<std::vector<uint8_t>> fetch() override;
    std::vector<InputVariableFloat::Info> floatVariableInfos() const override;
    std::vector<InputVariableEnum::Info> enumVariableInfos() const override;
    void setVariable(const std::string& variable, float value) override;
    void setVariable(const std::string& variable, const std::string& value) override;

    std::string fileName() const;

private:
    std::string m_sceneName;
    std::string m_fileName;
    std::vector<InputVariableFloat> m_floatVariables;
    std::vector<InputVariableEnum> m_enumVariables;
    std::shared_ptr<LazyRpcClient> m_rpc;
    bool m_dirty;
};