#include "Scene/UpdateDataDispatcher.h"

#include "Scene/DownloadProvider.h"

void UpdateDataDispatcher::dispatch(DownloadProvider& provider) {
    m_data = provider.download();
}

void UpdateDataDispatcher::dispatch(SCIRunProvider& provider) {
    // TODO
}

std::shared_ptr<std::vector<uint8_t>> UpdateDataDispatcher::data() const {
    return m_data;
}
