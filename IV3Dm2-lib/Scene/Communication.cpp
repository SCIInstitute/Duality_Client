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
//
//ServerAdapter::ServerAdapter(mocca::net::Endpoint& endpoint, const Scene* scene)
//    : m_endpoint(endpoint)
//    , m_rpcClient(nullptr)
//    , m_scene(scene) {}
//
//JsonCpp::Value ServerAdapter::scenes() const {
//    if (m_rpcClient == nullptr) {
//        m_rpcClient = std::make_unique<mocca::net::RpcClient>(m_endpoint);
//    }
//    m_rpcClient->send("listScenes", JsonCpp::Value());
//    return m_rpcClient->receive().first;
//}
//
//std::shared_ptr<std::vector<uint8_t>> ServerAdapter::download(const std::string& filename) const {
//    if (m_rpcClient == nullptr) {
//        m_rpcClient = std::make_unique<mocca::net::RpcClient>(m_endpoint);
//    }
//    JsonCpp::Value params;
//    params["scene"] = m_scene->metadata().name();
//    params["filename"] = filename;
//    m_rpcClient->send("download", params);
//    auto reply = m_rpcClient->receive();
//    return reply.second[0];
//}
//
//std::shared_ptr<std::vector<uint8_t>> ServerAdapter::sciRunGenerate(const std::string& filename, const JsonCpp::Value& values) const {
//    if (m_rpcClient == nullptr) {
//        m_rpcClient = std::make_unique<mocca::net::RpcClient>(m_endpoint);
//    }
//    JsonCpp::Value params;
//    params["scene"] = m_scene->metadata().name();
//    params["filename"] = filename;
//    params["values"] = values;
//    m_rpcClient->send("scirun", params);
//    auto reply = m_rpcClient->receive();
//    return reply.second[0];
//}
//
