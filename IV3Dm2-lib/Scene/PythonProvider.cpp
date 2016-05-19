#include "Scene/PythonProvider.h"

#include "Common/Error.h"

#include <algorithm>

PythonProvider::PythonProvider(const std::string& sceneName, const std::string& fileName, std::vector<InputVariableFloat> floatVariables,
                               std::vector<InputVariableEnum> enumVariables, std::shared_ptr<LazyRpcClient> rpc)
    : m_sceneName(sceneName)
    , m_fileName(fileName)
    , m_floatVariables(std::move(floatVariables))
    , m_enumVariables(std::move(enumVariables))
    , m_rpc(rpc)
    , m_dirty(true) {}

std::shared_ptr<std::vector<uint8_t>> PythonProvider::fetch() {
    if (!m_dirty) {
        return nullptr;
    }

    JsonCpp::Value values;
    for (const auto& var : m_floatVariables) {
        values[var.info().name] = var.value();
    }
    for (const auto& var : m_enumVariables) {
        values[var.info().name] = var.value();
    }

    JsonCpp::Value params;
    params["scene"] = m_sceneName;
    params["filename"] = m_fileName;
    params["variables"] = values;
    m_rpc->send("python", params);
    auto reply = m_rpc->receive();
    return reply.second[0];
}

std::vector<InputVariableFloat*> PythonProvider::inputVariablesFloat() {
    std::vector<InputVariableFloat*> vars;
    for (auto& var : m_floatVariables) {
        vars.push_back(&var);
    }
    return vars;
}

std::vector<InputVariableEnum*> PythonProvider::inputVariablesEnum() {
    std::vector<InputVariableEnum*> vars;
    for (auto& var : m_enumVariables) {
        vars.push_back(&var);
    }
    return vars;
}

std::string PythonProvider::fileName() const {
    return m_fileName;
}
