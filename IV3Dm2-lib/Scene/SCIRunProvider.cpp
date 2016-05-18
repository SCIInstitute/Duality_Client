#include "Scene/SCIRunProvider.h"

#include "Common/Error.h"

#include <algorithm>

SCIRunProvider::SCIRunProvider(const ServerAdapter* server, std::string network, std::vector<InputVariableFloat> floatVariables,
                               std::vector<InputVariableEnum> enumVariables)
    : m_server(server)
    , m_network(std::move(network))
    , m_floatVariables(std::move(floatVariables))
    , m_enumVariables(std::move(enumVariables))
    , m_dirty(true) {}

void SCIRunProvider::accept(DataProviderDispatcher& dispatcher) {
    dispatcher.dispatch(*this);
}

std::string SCIRunProvider::network() const {
    return m_network;
}

std::shared_ptr<std::vector<uint8_t>> SCIRunProvider::fetch() {
    if (!m_dirty) {
        return nullptr;
    }

    JsonCpp::Value params;
    for (const auto& var : m_floatVariables) {
        params[var.info().name] = var.value();
    }
    for (const auto& var : m_enumVariables) {
        params[var.info().name] = var.value();
    }
    return m_server->sciRunGenerate(m_network, params);
}

std::vector<InputVariableFloat*> SCIRunProvider::inputVariablesFloat() {
    std::vector<InputVariableFloat*> vars;
    for (auto& var : m_floatVariables) {
        vars.push_back(&var);
    }
    return vars;
}

std::vector<InputVariableEnum*> SCIRunProvider::inputVariablesEnum() {
    std::vector<InputVariableEnum*> vars;
    for (auto& var : m_enumVariables) {
        vars.push_back(&var);
    }
    return vars;
}
