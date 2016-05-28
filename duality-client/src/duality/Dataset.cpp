#include "src/duality/Dataset.h"

Dataset::Dataset(std::vector<IVDA::Mat4f> transforms)
    : m_transforms(std::move(transforms)) {}

Dataset::~Dataset() = default;

void Dataset::load(std::shared_ptr<std::vector<uint8_t>> data) {
    read(data);
    for (const auto& transform : m_transforms) {
        applyTransform(transform);
    }
}

std::vector<IVDA::Mat4f> Dataset::transforms() const {
    return m_transforms;
}
