#include "Scene/UpdateDataDispatcher.h"

#include "Scene/DownloadProvider.h"
#include "Scene/SCIRunProvider.h"

void UpdateDataDispatcher::dispatch(DownloadProvider& provider) {
    m_data = provider.download();
}

void UpdateDataDispatcher::dispatch(SCIRunProvider& provider) {
    if (provider.isDirty()) {
        m_data = provider.generate();
        provider.setDirty(false);
    }
}

std::shared_ptr<std::vector<uint8_t>> UpdateDataDispatcher::data() const {
    return m_data;
}
