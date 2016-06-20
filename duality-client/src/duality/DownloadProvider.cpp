#include "src/duality/DownloadProvider.h"

#include "duality/Error.h"

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
    if (reply.second.empty()) {
        throw Error("Could not download file '" + m_fileName + "'", __FILE__, __LINE__);
    }
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
