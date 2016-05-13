#include "Scene/SCIRunProvider.h"

#include "Common/Error.h"

#include <algorithm>

SCIRunProvider::SCIRunProvider(const ServerAdapter* server, std::string network, std::vector<InputParameterFloat> floatParameters,
                               std::vector<InputParameterEnum> enumParameters)
    : m_server(server)
    , m_network(std::move(network))
    , m_floatParameters(std::move(floatParameters))
    , m_enumParameters(std::move(enumParameters))
    , m_dirty(true) {
    for (const auto& param : m_floatParameters) {
        m_floatValues[param.name] = param.defaultValue;
    }
}

void SCIRunProvider::accept(DataProviderDispatcher& dispatcher) {
    dispatcher.dispatch(*this);
}

std::string SCIRunProvider::name() const {
    return m_network;
}

std::vector<InputParameterFloat> SCIRunProvider::floatParameters() const {
    return m_floatParameters;
}

std::vector<InputParameterEnum> SCIRunProvider::enumParameters() const {
    return m_enumParameters;
}

void SCIRunProvider::setFloatValue(const std::string& name, float value) {
    auto it = std::find_if(begin(m_floatParameters), end(m_floatParameters), [&](const InputParameterFloat& p) { return p.name == name; });
    if (it == end(m_floatParameters)) {
        throw Error("Invalid float parameter '" + name + "'", __FILE__, __LINE__);
    }
    if (m_floatValues[name] != value) {
        m_floatValues[name] = value;
        m_dirty = true;
    }
}

std::map<std::string, float> SCIRunProvider::floatValues() const {
    return m_floatValues;
}

std::shared_ptr<std::vector<uint8_t>> SCIRunProvider::generate() const {
    JsonCpp::Value params;
    for (const auto& val : m_floatValues) {
        params[val.first] = val.second;
    }
    return m_server->sciRunGenerate(m_network, params);
}

bool SCIRunProvider::isDirty() const {
    return m_dirty;
}

void SCIRunProvider::setDirty(bool dirty) {
    m_dirty = dirty;
}