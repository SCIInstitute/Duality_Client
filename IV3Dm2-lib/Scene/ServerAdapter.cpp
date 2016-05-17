//  Created by David McCann on 5/4/16.
//  Copyright © 2016 Scientific Computing and Imaging Institute. All rights reserved.
//

#include "ServerAdapter.h"

ServerAdapter::ServerAdapter(const std::string& ip, uint16_t port)
    : m_endpoint("tcp.prefixed", ip, std::to_string(port))
    , m_rpcClient(nullptr) {}

JsonCpp::Value ServerAdapter::scenes() const {
    if (m_rpcClient == nullptr) {
        m_rpcClient = std::make_unique<mocca::net::RpcClient>(m_endpoint);
    }
    m_rpcClient->send("listScenes", JsonCpp::Value());
    return m_rpcClient->receive().first;
}

std::shared_ptr<std::vector<uint8_t>> ServerAdapter::download(const std::string& path) const {
    if (m_rpcClient == nullptr) {
        m_rpcClient = std::make_unique<mocca::net::RpcClient>(m_endpoint);
    }
    JsonCpp::Value params;
    params["path"] = path;
    m_rpcClient->send("download", params);
    auto reply = m_rpcClient->receive();
    return reply.second[0];
}

std::shared_ptr<std::vector<uint8_t>> ServerAdapter::sciRunGenerate(const std::string& network, const JsonCpp::Value& sciRunParams) const {
    if (m_rpcClient == nullptr) {
        m_rpcClient = std::make_unique<mocca::net::RpcClient>(m_endpoint);
    }
    JsonCpp::Value params;
    params["params"] = sciRunParams;
    m_rpcClient->send("scirun", params);
    auto reply = m_rpcClient->receive();
    return reply.second[0];
}
