#pragma once

#include "Scene/DataProviderDispatcher.h"

#include <memory>
#include <vector>

class UpdateDataDispatcher : public DataProviderDispatcher {
public:
    void dispatch(DownloadProvider& provider) override;
    void dispatch(SCIRunProvider& provider) override;

    std::shared_ptr<std::vector<uint8_t>> data() const;

private:
    std::shared_ptr<std::vector<uint8_t>> m_data;
};