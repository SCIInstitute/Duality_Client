#include "src/duality/DownloadProvider.h"

#include "duality/Error.h"

#include <cassert>

DownloadProvider::DownloadProvider(std::string sceneName, std::string fileName, std::shared_ptr<LazyRpcClient> rpc,
                                   std::shared_ptr<DataCache> cache)
    : m_sceneName(sceneName)
    , m_fileName(fileName)
    , m_rpc(rpc)
    , m_cache(cache)
    , m_dirty(true) {}

std::shared_ptr<std::vector<uint8_t>> DownloadProvider::fetch() {
    if (!m_dirty) {
        return nullptr;
    }
    
    m_dirty = false;
    
    auto cachedData = m_cache->fetch(cacheID());
    if (cachedData != nullptr) {
        return cachedData;
    }
    
    JsonCpp::Value params;
    params["scene"] = m_sceneName;
    params["filename"] = m_fileName;
    m_rpc->send("download", params);
    auto reply = m_rpc->receive();
    if (reply.second.empty()) {
        throw Error("Could not download file '" + m_fileName + "'", __FILE__, __LINE__);
    }
    
    m_cache->write(cacheID(), *reply.second[0]);
    
    return reply.second[0];
}

JsonCpp::Value DownloadProvider::cacheID() const {
    JsonCpp::Value id;
    id["type"] = "download";
    id["scene"] = m_sceneName;
    id["file"] = m_fileName;
    return id;
}

std::string DownloadProvider::fileName() const {
    return m_fileName;
}
