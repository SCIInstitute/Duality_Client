#include "Scene/PythonProvider.h"

#include "Common/Error.h"

#include <algorithm>
#include <cassert>

PythonProvider::PythonProvider(const std::string& sceneName, const std::string& fileName, std::vector<InputVariableFloat> floatVariables,
                               std::vector<InputVariableEnum> enumVariables, std::shared_ptr<LazyRpcClient> rpc)
    : m_sceneName(sceneName)
    , m_fileName(fileName)
    , m_floatVariables(std::move(floatVariables))
    , m_enumVariables(std::move(enumVariables))
    , m_rpc(rpc)
    , m_dirty(true) {}

std::shared_ptr<std::vector<uint8_t>> PythonProvider::fetch() {
    if (!m_dirty)
        return nullptr;

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
    m_dirty = false;
    return reply.second[0];
}


std::vector<InputVariableFloat::Info> PythonProvider::floatVariableInfos() const {
    std::vector<InputVariableFloat::Info> result;
    for (const auto& var : m_floatVariables) {
        result.push_back(var.info());
    }
    return result;
}

std::vector<InputVariableEnum::Info> PythonProvider::enumVariableInfos() const {
    std::vector<InputVariableEnum::Info> result;
    for (const auto& var : m_enumVariables) {
        result.push_back(var.info());
    }
    return result;
}

void PythonProvider::setVariable(const InputVariableFloat& variable) {
    std::replace_if(begin(m_floatVariables), end(m_floatVariables),
                    [&variable](const InputVariableFloat& var) { return var.info().name == variable.info().name; }, variable);
    m_dirty = true;
}

void PythonProvider::setVariable(const InputVariableEnum &variable) {
    std::replace_if(begin(m_enumVariables), end(m_enumVariables),
                    [&variable](const InputVariableEnum& var) { return var.info().name == variable.info().name; }, variable);
    m_dirty = true;
}

std::vector<DataProvider::InputSetter<InputVariableFloat>> PythonProvider::floatSetters() {
    std::vector<DataProvider::InputSetter<InputVariableFloat>> result;
    for (const auto& var : m_floatVariables) {
        result.emplace_back(var, this);
    }
    return result;
}

std::vector<DataProvider::InputSetter<InputVariableEnum>> PythonProvider::enumSetters() {
    std::vector<DataProvider::InputSetter<InputVariableEnum>> result;
    for (const auto& var : m_enumVariables) {
        result.emplace_back(var, this);
    }
    return result;
}

std::string PythonProvider::fileName() const {
    return m_fileName;
}
