#include "Scene/UpdateDataDispatcher.h"

#include "Scene/DownloadProvider.h"
#include "Scene/SCIRunProvider.h"

void UpdateDataDispatcher::dispatch(DownloadProvider& provider) {
    m_data = provider.download();
}

void UpdateDataDispatcher::dispatch(SCIRunProvider& provider) {
    auto data = provider.generate();
    if (data != nullptr) {
        m_data = data;
    }
}

std::shared_ptr<std::vector<uint8_t>> UpdateDataDispatcher::data() const {
    return m_data;
}
