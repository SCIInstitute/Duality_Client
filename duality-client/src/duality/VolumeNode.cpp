#include "src/duality/VolumeNode.h"

VolumeNode::VolumeNode(const std::string& name, Visibility visibility, std::unique_ptr<VolumeDataset> dataset,
                       std::unique_ptr<TransferFunction> tf)
    : SceneNode(name, visibility)
    , m_dataset(std::move(dataset))
    , m_tf(std::move(tf))
    , m_updateEnabled(true) {}

void VolumeNode::render(RenderDispatcher2D& dispatcher) {
    dispatcher.dispatch(*this);
}

void VolumeNode::render(RenderDispatcher3D& dispatcher) {
    dispatcher.dispatch(*this);
}

void VolumeNode::setUpdateEnabled(bool enabled) {
    m_updateEnabled = enabled;
}

void VolumeNode::updateDataset() {
    if (m_updateEnabled) {
        m_dataset->updateDataset();
        m_tf->update();
    }
}

void VolumeNode::initializeDataset() {
    if (m_updateEnabled) {
        m_dataset->initializeDataset();
        m_tf->initTexture();
    }
}

BoundingBox VolumeNode::boundingBox() const {
    return m_dataset->boundingBox();
}

const VolumeDataset& VolumeNode::dataset() const {
    return *m_dataset;
}

const TransferFunction& VolumeNode::transferFunction() const {
    return *m_tf;
}
