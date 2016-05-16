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

std::string SCIRunProvider::network() const {
    return m_network;
}

std::vector<InputParameterFloat> SCIRunProvider::floatParameters() const {
    return m_floatParameters;
}

std::vector<InputParameterEnum> SCIRunProvider::enumParameters() const {
    return m_enumParameters;
}

void SCIRunProvider::setFloatValue(const std::string& name, float value) {
    m_floatValues[name] = value;
    m_dirty = true;
}

float SCIRunProvider::getFloatValue(const std::string& name) const {
    auto it = m_floatValues.find(name);
    if (it == end(m_floatValues)) {
        throw Error("Unknown float parameter '" + name + "'", __FILE__, __LINE__);
    }
    return it->second;
}

void SCIRunProvider::setEnumValue(const std::string& name, const std::string& value) {
    assert(std::find(begin(m_parameter.values), end(m_parameter.values), value) != end(m_parameter.values));
    m_enumValues[name] = value;
}

std::string SCIRunProvider::getEnumValue(const std::string& name) const {
    auto it = m_enumValues.find(name);
    if (it == end(m_enumValues)) {
        throw Error("Unknown enum parameter '" + name + "'", __FILE__, __LINE__);
    }
    return it->second;
}

std::shared_ptr<std::vector<uint8_t>> SCIRunProvider::generate() const {
    if (!m_dirty) {
        return nullptr;
    }

    JsonCpp::Value params;
    for (const auto& val : m_floatValues) {
        params[val.first] = val.second;
    }
    return m_server->sciRunGenerate(m_network, params);
}