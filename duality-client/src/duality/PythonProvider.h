#pragma once

#include "duality/InputVariable.h"

#include "src/duality/Communication.h"
#include "src/duality/DataCache.h"
#include "src/duality/DataProvider.h"

#include "mocca/base/Nullable.h"

#include <memory>
#include <vector>

class PythonProvider : public DataProvider {
public:
    PythonProvider(const std::string& sceneName, const std::string& fileName, std::shared_ptr<Variables> variables,
                   std::shared_ptr<LazyRpcClient> rpc, std::shared_ptr<DataCache> cache);

    // DataProvider interface
    std::shared_ptr<std::vector<uint8_t>> fetch() override;
    void notify() override;

    std::string fileName() const;

private:
    JsonCpp::Value cacheID() const;
    bool isFetchRequired() const;

private:
    std::string m_sceneName;
    std::string m_fileName;
    std::shared_ptr<Variables> m_variables;
    mocca::Nullable<Variables> m_currentVariables;
    std::shared_ptr<LazyRpcClient> m_rpc;
    std::shared_ptr<DataCache> m_cache;
};