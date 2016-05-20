//  Created by David McCann on 5/4/16.
//  Copyright © 2016 Scientific Computing and Imaging Institute. All rights reserved.
//

#include "Scene/Communication.h"

#include "mocca/net/NetworkError.h"

LazyRpcClient::LazyRpcClient(const mocca::net::Endpoint& endpoint)
    : m_endpoint(endpoint)
    , m_rpcClient(nullptr) {}

void LazyRpcClient::send(const std::string& method, const JsonCpp::Value& params) const {
    try {
        if (m_rpcClient == nullptr) {
            m_rpcClient = std::make_unique<mocca::net::RpcClient>(m_endpoint);
        }
        m_rpcClient->send(method, params);
    } catch (const mocca::net::NetworkError& err) {
        m_rpcClient = nullptr;
        throw err;
    }
}

mocca::net::RpcClient::ReturnType LazyRpcClient::receive() const {
    try {
        if (m_rpcClient == nullptr) {
            m_rpcClient = std::make_unique<mocca::net::RpcClient>(m_endpoint);
        }
        return m_rpcClient->receive();
    } catch (const mocca::net::NetworkError& err) {
        m_rpcClient = nullptr;
        throw err;
    }
}
