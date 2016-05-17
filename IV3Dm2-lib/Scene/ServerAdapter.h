//  Created by David McCann on 5/4/16.
//  Copyright © 2016 Scientific Computing and Imaging Institute. All rights reserved.
//

#pragma once

#include "SceneLoader.h"

#include "mocca/net/rpc/RpcClient.h"

#include "jsoncpp/json.h"

#include <memory>
#include <vector>

class ServerAdapter {
public:
    ServerAdapter(const std::string& ip, uint16_t port);

    JsonCpp::Value scenes() const;
    std::shared_ptr<std::vector<uint8_t>> download(const std::string& path) const;
    std::shared_ptr<std::vector<uint8_t>> sciRunGenerate(const std::string& network, const JsonCpp::Value& sciRunParams) const;

private:
    mocca::net::Endpoint m_endpoint;
    mutable std::unique_ptr<mocca::net::RpcClient> m_rpcClient;
};