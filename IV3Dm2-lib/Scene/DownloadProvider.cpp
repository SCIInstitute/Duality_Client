//  Created by David McCann on 5/5/16.
//  Copyright © 2016 Scientific Computing and Imaging Institute. All rights reserved.
//

#include "Scene/DownloadProvider.h"

#include <cassert>

DownloadProvider::DownloadProvider(std::string sceneName, std::string fileName, std::shared_ptr<LazyRpcClient> rpc)
    : m_sceneName(sceneName)
    , m_fileName(fileName)
    , m_rpc(rpc) {}

std::shared_ptr<std::vector<uint8_t>> DownloadProvider::fetch() {
    JsonCpp::Value params;
    params["scene"] = m_sceneName;
    params["filename"] = m_fileName;
    m_rpc->send("download", params);
    auto reply = m_rpc->receive();
    return reply.second[0];
}

std::string DownloadProvider::fileName() const {
    return m_fileName;
}
