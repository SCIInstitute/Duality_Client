#include "src/duality/Dataset.h"

Dataset::Dataset(std::unique_ptr<DataProvider> provider)
    : m_provider(std::move(provider)) {}

void Dataset::fetch() {
    auto data = m_provider->fetch();
    if (data != nullptr) {
        readData(*data);
    }
}