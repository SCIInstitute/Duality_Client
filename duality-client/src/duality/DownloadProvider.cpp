//  Created by David McCann on 5/5/16.
//  Copyright © 2016 Scientific Computing and Imaging Institute. All rights reserved.
//

#include "src/duality/DownloadProvider.h"

#include <cassert>

DownloadProvider::DownloadProvider(std::string sceneName, std::string fileName, std::shared_ptr<LazyRpcClient> rpc)
    : m_sceneName(sceneName)
    , m_fileName(fileName)
    , m_rpc(rpc)
    , m_dirty(true) {}

std::shared_ptr<std::vector<uint8_t>> DownloadProvider::fetch() {
    if (!m_dirty) {
        return nullptr;
    }
    JsonCpp::Value params;
    params["scene"] = m_sceneName;
    params["filename"] = m_fileName;
    m_rpc->send("download", params);
    auto reply = m_rpc->receive();
    m_dirty = false;
    return reply.second[0];
}

std::string DownloadProvider::fileName() const {
    return m_fileName;
}
