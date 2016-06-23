#pragma once

#include "mocca/fs/Path.h"

#include "jsoncpp/json.h"

#include <memory>

class DataCache {
public:
    enum class Mode { Enabled, Disabled };
    DataCache(const mocca::fs::Path& cacheDir, Mode mode = Mode::Enabled);

    void setMode(Mode mode);
    std::shared_ptr<std::vector<uint8_t>> fetch(const JsonCpp::Value& cacheID);
    void write(const JsonCpp::Value& cacheID, const std::vector<uint8_t>& data);
    void clear();
    
private:
    mocca::fs::Path m_cacheDir;
    Mode m_mode;
};