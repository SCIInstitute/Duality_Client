//  Created by David McCann on 5/4/16.
//  Copyright © 2016 Scientific Computing and Imaging Institute. All rights reserved.
//

#include "Scene/Dataset.h"
#include "ServerAdapter.h"

#include "mocca/net/rpc/RpcClient.h"

ServerAdapter::ServerAdapter() {
    mocca::net::Endpoint ep("tcp.prefixed", "192.168.1.222", "10123");
    m_rpcClient = std::make_unique<mocca::net::RpcClient>(ep);
}

std::vector<SceneMetadata> ServerAdapter::listMetadata() const {
    m_rpcClient->send("listMetadata", JsonCpp::Value());
    auto reply = m_rpcClient->receive().first;

    std::vector<SceneMetadata> result;
    for (auto it = reply.begin(); it != reply.end(); ++it) {
        result.push_back(SceneMetadata::fromJson(*it));
    }
    return result;
}

Scene ServerAdapter::getScene(const std::string& name) const {
    JsonCpp::Value params;
    params["name"] = name;
    m_rpcClient->send("getScene", params);
    auto reply = m_rpcClient->receive().first;
    return Scene::fromJson(reply);
}

std::unique_ptr<Dataset> ServerAdapter::downloadDataset(const std::string& path) const {
    JsonCpp::Value params;
    params["path"] = path;
    m_rpcClient->send("download", params);
    auto reply = m_rpcClient->receive();
    return Dataset::create(*reply.second[0]);
}
