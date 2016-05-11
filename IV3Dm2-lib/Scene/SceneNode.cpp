#include "Scene/SceneNode.h"

SceneNode::SceneNode(std::unique_ptr<DataProvider> provider, std::vector<IVDA::Mat4f> transforms)
    : m_provider(std::move(provider))
    , m_transforms(std::move(transforms)) {}

void SceneNode::updateDataset() {
    if (m_provider != nullptr) {
        auto data = m_provider->fetch();
        if (data != nullptr) {
            readDataset(data);
            for (const auto& transform : m_transforms) {
                applyTransform(transform);
            }
        }
    }
}

const DataProvider& SceneNode::dataProvider() const noexcept {
    return *m_provider;
}

const std::vector<IVDA::Mat4f>& SceneNode::transforms() const noexcept {
    return m_transforms;
}
