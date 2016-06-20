#pragma once

#include "src/duality/Communication.h"
#include "src/duality/DataCache.h"
#include "src/duality/DataProvider.h"

#include <memory>
#include <string>
#include <vector>

class DownloadProvider : public DataProvider {
public:
    DownloadProvider(std::string sceneName, std::string fileName, std::shared_ptr<LazyRpcClient> rpc, std::shared_ptr<DataCache> cache);

    // DataProvider interface
    std::shared_ptr<std::vector<uint8_t>> fetch() override;

    std::string fileName() const;

private:
    JsonCpp::Value cacheID() const;

private:
    std::string m_sceneName;
    std::string m_fileName;
    std::shared_ptr<LazyRpcClient> m_rpc;
    std::shared_ptr<DataCache> m_cache;
    bool m_dirty;
};