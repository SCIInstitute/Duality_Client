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

std::vector<SceneDefinition> ServerAdapter::fetchDefinitions() const {
    m_rpcClient->send("ListScenes", JsonCpp::Value());
    auto reply = m_rpcClient->receive().first;

    std::vector<SceneDefinition> result;
    for (auto it = reply.begin(); it != reply.end(); ++it) {
        result.push_back(SceneDefinition::fromJson(*it));
    }
    return result;
}

std::unique_ptr<Dataset> ServerAdapter::downloadDataset(const std::string& path) const {
    JsonCpp::Value params;
    params["path"] = path;
    m_rpcClient->send("Download", params);
    auto reply = m_rpcClient->receive();
    return Dataset::create(*reply.second[0]);
}
