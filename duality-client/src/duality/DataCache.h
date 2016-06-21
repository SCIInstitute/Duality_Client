#pragma once

#include "mocca/fs/Path.h"

#include "jsoncpp/json.h"

#include <memory>

class DataCache {
public:
    DataCache(const mocca::fs::Path& cacheDir);

    std::shared_ptr<std::vector<uint8_t>> fetch(const JsonCpp::Value& cacheID);
    void write(const JsonCpp::Value& cacheID, const std::vector<uint8_t>& data);
    void clear();
    
private:
    mocca::fs::Path m_cacheDir;
};