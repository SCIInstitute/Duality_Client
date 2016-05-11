#include "Scene/SceneNode.h"

SceneNode::SceneNode(std::unique_ptr<DataProvider> provider, MatrixTriple transforms)
    : m_provider(std::move(provider))
    , m_transforms(std::move(transforms)) {}

void SceneNode::updateDataset() {
    if (m_provider != nullptr) {
        auto data = m_provider->fetch();
        if (data != nullptr) {
            readDataset(data);
            if (std::get<2>(m_transforms)) {
                // first scale the object
                applyTransform(*std::get<2>(m_transforms));
            }
            if (std::get<1>(m_transforms)) {
                // then rotate the object
                applyTransform(*std::get<1>(m_transforms));
            }
            if (std::get<0>(m_transforms)) {
                // then translate the object
                applyTransform(*std::get<2>(m_transforms));
            }
        }
    }
}

const DataProvider& SceneNode::dataProvider() const noexcept {
    return *m_provider;
}

const SceneNode::MatrixTriple& SceneNode::transforms() const noexcept {
    return m_transforms;
}
