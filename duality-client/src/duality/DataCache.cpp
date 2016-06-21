#include "src/duality/DataCache.h"

#include "duality/Error.h"

#include "mocca/base/Compression.h"
#include "mocca/base/StringTools.h"
#include "mocca/fs/Filesystem.h"
#include "mocca/log/LogManager.h"

DataCache::DataCache(const mocca::fs::Path& cacheDir)
    : m_cacheDir(cacheDir) {}

std::shared_ptr<std::vector<uint8_t>> DataCache::fetch(const JsonCpp::Value& cacheID) {
    auto sceneCacheDir = m_cacheDir + cacheID["scene"].asString();
    if (!mocca::fs::exists(sceneCacheDir)) {
        return nullptr;
    }
    auto dirs = mocca::fs::directoryContents(sceneCacheDir);
    for (const auto& dir : dirs) {
        auto cacheIDFile = dir + "cacheID.txt";
        if (!mocca::fs::exists(cacheIDFile)) {
            throw Error(MAKE_STRING("Cache ID file " << cacheIDFile << " does not exist. Cache corrupted."), __FILE__, __LINE__);
        }
        JsonCpp::Reader reader;
        JsonCpp::Value otherCacheID;
        reader.parse(mocca::fs::readTextFile(cacheIDFile), otherCacheID);
        // toStyledString is a workaround; the actual comparison operator gives the wrong result when comparing int and float values
        if (cacheID.toStyledString() == otherCacheID.toStyledString()) {
            LINFO("Cached object found");
            auto dataFile = dir + "data.bin";
            auto uncompressedData = mocca::uncompressData(*mocca::fs::readBinaryFile(dataFile));
            return std::shared_ptr<std::vector<uint8_t>>(uncompressedData.release());
        }
    }
    return nullptr;
}

void DataCache::write(const JsonCpp::Value& cacheID, const std::vector<uint8_t>& data) {
    // create new directory with unique name
    auto sceneCacheDir = m_cacheDir + cacheID["scene"].asString();
    int i = 1;
    auto newDir = sceneCacheDir + std::to_string(i);
    while (mocca::fs::exists(newDir)) {
        ++i;
        newDir = sceneCacheDir + std::to_string(i);
    }
    mocca::fs::createDirectories(newDir);

    // write cache file
    auto cacheIDPath = newDir + "cacheID.txt";
    JsonCpp::StyledWriter writer;
    mocca::fs::writeTextFile(cacheIDPath, writer.write(cacheID));

    // write binary file
    auto dataPath = newDir + "data.bin";
    mocca::fs::writeBinaryFile(dataPath, *mocca::compressData(data));
}

void DataCache::clear() {
    mocca::fs::removeDirectoryRecursive(m_cacheDir);
}
