#include "src/duality/PythonProvider.h"

#include "duality/Error.h"

#include "mocca/base/StringTools.h"

#include <algorithm>
#include <cassert>

PythonProvider::PythonProvider(const std::string& sceneName, const std::string& fileName, std::shared_ptr<Variables> variables,
                               std::shared_ptr<LazyRpcClient> rpc, std::shared_ptr<DataCache> cache)
    : m_sceneName(sceneName)
    , m_fileName(fileName)
    , m_variables(variables)
    , m_currentVariables()
    , m_rpc(rpc)
    , m_cache(cache) {}

std::shared_ptr<std::vector<uint8_t>> PythonProvider::fetch() {
    if (!isFetchRequired()) {
        return nullptr;
    }
    
    m_currentVariables = *m_variables;

    auto cachedData = m_cache->fetch(cacheID());
    if (cachedData != nullptr) {
        return cachedData;
    }

    JsonCpp::Value values;
    for (const auto& var : m_variables->floatVariables) {
        values[var.name] = var.value;
    }
    for (const auto& var : m_variables->enumVariables) {
        values[var.name] = var.value;
    }

    JsonCpp::Value params;
    params["scene"] = m_sceneName;
    params["filename"] = m_fileName;
    params["variables"] = values;
    m_rpc->send("python", params);
    auto reply = m_rpc->receive();
    if (reply.second.empty()) {
        throw Error(MAKE_STRING("Python script '" << m_fileName << "' did not return any data"), __FILE__, __LINE__);
    }
    
    m_cache->write(cacheID(), *reply.second[0]);
    
    return reply.second[0];
}

std::string PythonProvider::fileName() const {
    return m_fileName;
}

JsonCpp::Value PythonProvider::cacheID() const {
    JsonCpp::Value id;
    id["type"] = "python";
    id["scene"] = m_sceneName;
    id["file"] = m_fileName;
    JsonCpp::Value values;
    for (const auto& var : m_variables->floatVariables) {
        values[var.name] = var.value;
    }
    for (const auto& var : m_variables->enumVariables) {
        values[var.name] = var.value;
    }
    id["variables"] = values;
    return id;
}

bool PythonProvider::isFetchRequired() const {
    if (m_currentVariables.isNull()) {
        return true;
    }
    const auto& current = m_currentVariables.get();
    assert(current.floatVariables.size() == m_variables->floatVariables.size() &&
           current.enumVariables.size() == m_variables->enumVariables.size());
    return !std::equal(begin(current.floatVariables), end(current.floatVariables), begin(m_variables->floatVariables),
                       [](const FloatVariable& lhs, const FloatVariable& rhs) { return lhs.value == rhs.value; }) ||
           !std::equal(begin(current.enumVariables), end(current.enumVariables), begin(m_variables->enumVariables),
                       [](const EnumVariable& lhs, const EnumVariable& rhs) { return lhs.value == rhs.value; });
}