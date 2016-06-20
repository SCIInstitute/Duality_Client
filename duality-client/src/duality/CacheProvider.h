#pragma once

#include "mocca/fs/Path.h"

#include "src/duality/DataProvider.h"

class CacheProvider : public DataProvider {
public:
    CacheProvider(const mocca::fs::Path& cacheDir);

    virtual std::shared_ptr<std::vector<uint8_t>> fetch() override;
    virtual JsonCpp::Value cacheID() const override;

private:
    mocca::fs::Path m_cacheDir;
    std::unique_ptr<DataProvider> m_provider;
};