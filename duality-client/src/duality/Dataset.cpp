#include "src/duality/Dataset.h"

Dataset::Dataset(std::vector<IVDA::Mat4f> transforms, Visibility visibility)
    : m_transforms(std::move(transforms))
    , m_visibility(visibility) {}

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

bool Dataset::isVisibleInView(View view) const {
    return (static_cast<int>(m_visibility) & static_cast<int>(view)) != 0;
}