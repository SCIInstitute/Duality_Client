#pragma once

#include "src/duality/Communication.h"
#include "src/duality/DataProvider.h"

#include <string>
#include <memory>
#include <vector>

class DownloadProvider : public DataProvider {
public:
    DownloadProvider(std::string sceneName, std::string fileName, std::shared_ptr<LazyRpcClient> rpc);

    // DataProvider interface
    std::shared_ptr<std::vector<uint8_t>> fetch() override;
    JsonCpp::Value cacheID() const override;

    std::string fileName() const;

private:
    std::string m_sceneName;
    std::string m_fileName;
    std::shared_ptr<LazyRpcClient> m_rpc;
};