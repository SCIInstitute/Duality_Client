//  Created by David McCann on 5/4/16.
//  Copyright © 2016 Scientific Computing and Imaging Institute. All rights reserved.
//

#include "ServerAdapter.h"

ServerAdapter::ServerAdapter() {
    mocca::net::Endpoint ep("tcp.prefixed", "192.168.1.222", "10123");
    m_rpcClient = std::make_unique<mocca::net::RpcClient>(ep);
}

std::vector<Scene> ServerAdapter::listScenes() const {
    m_rpcClient->send("listScenes", JsonCpp::Value());
    auto reply = m_rpcClient->receive().first;

    std::vector<Scene> result;
    for (auto it = reply.begin(); it != reply.end(); ++it) {
        result.push_back(Scene::fromJson(*it));
    }
    return result;
}

std::shared_ptr<std::vector<uint8_t>> ServerAdapter::download(const std::string& path) const {
    JsonCpp::Value params;
    params["path"] = path;
    m_rpcClient->send("download", params);
    auto reply = m_rpcClient->receive();
    return reply.second[0];
}
