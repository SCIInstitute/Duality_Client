#include "src/duality/GeometryNode.h"

GeometryNode::GeometryNode(const std::string& name, Visibility visibility, std::unique_ptr<GeometryDataset> dataset)
    : SceneNode(name, visibility)
    , m_dataset(std::move(dataset))
    , m_updateEnabled(true) {}

void GeometryNode::render(RenderDispatcher2D& dispatcher) {
    dispatcher.dispatch(*this);
}

void GeometryNode::render(RenderDispatcher3D& dispatcher) {
    dispatcher.dispatch(*this);
}

void GeometryNode::setUpdateEnabled(bool enabled) {
    m_updateEnabled = enabled;
}

void GeometryNode::updateDataset() {
    if (m_updateEnabled) {
        m_dataset->updateDataset();
    }
}

void GeometryNode::initializeDataset() {
    if (m_updateEnabled) {
        m_dataset->initializeDataset();
    }
}

BoundingBox GeometryNode::boundingBox() const {
    return m_dataset->boundingBox();
}

bool GeometryNode::intersects(const BoundingBox& box) const {
    return m_dataset->intersects(box);
}

const GeometryDataset& GeometryNode::dataset() const {
    return *m_dataset;
}

bool GeometryNode::isTransparent() const {
    return m_dataset->isTransparent();
}