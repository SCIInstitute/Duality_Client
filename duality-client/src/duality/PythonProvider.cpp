#include "src/duality/PythonProvider.h"

#include "duality/Error.h"

#include "mocca/base/StringTools.h"

#include <algorithm>
#include <cassert>

PythonProvider::PythonProvider(const std::string& sceneName, const std::string& fileName, std::shared_ptr<Variables> variables,
                               std::shared_ptr<LazyRpcClient> rpc)
    : m_sceneName(sceneName)
    , m_fileName(fileName)
    , m_variables(variables)
    , m_currentVariables()
    , m_rpc(rpc) {}

std::shared_ptr<std::vector<uint8_t>> PythonProvider::fetch() {
    if (!isFetchRequired()) {
        return nullptr;
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
    m_currentVariables = *m_variables;
    return reply.second[0];
}

std::string PythonProvider::fileName() const {
    return m_fileName;
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
