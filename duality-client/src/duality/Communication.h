#pragma once

#include "mocca/net/rpc/RpcClient.h"

#include "jsoncpp/json.h"

#include <memory>
#include <vector>

class Scene;

class LazyRpcClient {
public:
    LazyRpcClient(const mocca::net::Endpoint& endpoint);
    void send(const std::string& method, const JsonCpp::Value& params) const;
    mocca::net::RpcClient::ReturnType receive() const;
private:
    mocca::net::Endpoint m_endpoint;
    mutable std::unique_ptr<mocca::net::RpcClient> m_rpcClient;
};