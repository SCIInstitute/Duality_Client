#include "Scene/SCIRunProvider.h"

SCIRunProvider::SCIRunProvider(std::string network, std::vector<InputParameterFloat> floatParameters,
                               std::vector<InputParameterEnum> enumParameters)
    : m_network(std::move(network))
    , m_floatParameters(std::move(floatParameters))
    , m_enumParameters(std::move(enumParameters)) {}

void SCIRunProvider::accept(DataProviderDispatcher& dispatcher) {
    // FIXME
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
