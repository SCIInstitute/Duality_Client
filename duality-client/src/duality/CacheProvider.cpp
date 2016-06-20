#include "src/duality/CacheProvider.h"

#include "duality/Error.h"

#include "mocca/fs/Filesystem.h"
#include "mocca/base/StringTools.h"
#include "mocca/log/LogManager.h"

CacheProvider::CacheProvider(const mocca::fs::Path& cacheDir)
    : m_cacheDir(cacheDir) {}

std::shared_ptr<std::vector<uint8_t>> CacheProvider::fetch() {
    auto dirs = mocca::fs::directoryContents(m_cacheDir);
    for (const auto& dir : dirs) {
        auto cacheIDFile = dir + "cacheID.txt";
        if (!mocca::fs::exists(cacheIDFile)) {
            throw Error(MAKE_STRING("Cache ID file " << cacheIDFile << " does not exist. Cache corrupted."), __FILE__, __LINE__);
        }
        JsonCpp::Reader reader;
        JsonCpp::Value cacheID;
        reader.parse(cacheIDFile, cacheID);
        if (cacheID == m_provider->cacheID()) {
            LINFO("Cached object found");
            auto dataFile = dir + "data.bin";
            return mocca::fs::readBinaryFile(dataFile);
        }
    }
    auto data = m_provider->fetch();

    // create new directory with unique name
    bool created = false;
    int i = 1;
    auto newDir = m_cacheDir + std::to_string(i);
    while (mocca::fs::exists(newDir)) {
        ++i;
        newDir = m_cacheDir + std::to_string(i);
    }

    // write binary file
    auto dataPath = newDir + "data.bin";
    mocca::fs::writeBinaryFile(dataPath, *data);

    // write cache file
    auto cacheIDPath = newDir + "cacheID.txt";
    JsonCpp::StyledWriter writer;
    auto cacheID = writer.write(m_provider->cacheID());
    mocca::fs::writeTextFile(cacheIDPath, cacheID);

    return data;
}

JsonCpp::Value CacheProvider::cacheID() const {
    throw std::logic_error("The method or operation is not implemented.");
}
