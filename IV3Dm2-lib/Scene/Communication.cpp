//  Created by David McCann on 5/4/16.
//  Copyright © 2016 Scientific Computing and Imaging Institute. All rights reserved.
//

#include "Scene/Communication.h"

LazyRpcClient::LazyRpcClient(const mocca::net::Endpoint& endpoint)
    : m_endpoint(endpoint), m_rpcClient(nullptr)
{
}

void LazyRpcClient::send(const std::string& method, const JsonCpp::Value& params) const
{
    if (m_rpcClient == nullptr) {
        m_rpcClient = std::make_unique<mocca::net::RpcClient>(m_endpoint);
    }
    m_rpcClient->send(method, params);
}

mocca::net::RpcClient::ReturnType LazyRpcClient::receive() const
{
    if (m_rpcClient == nullptr) {
        m_rpcClient = std::make_unique<mocca::net::RpcClient>(m_endpoint);
    }
    return m_rpcClient->receive();
}
