#include "src/duality/GeometryNode.h"

GeometryNode::GeometryNode(const std::string& name, Visibility visibility, std::unique_ptr<GeometryDataset> dataset)
    : SceneNode(name, visibility)
    , m_dataset(std::move(dataset)) {}

void GeometryNode::render(RenderDispatcher2D& dispatcher) {
    dispatcher.dispatch(*this);
}

void GeometryNode::render(RenderDispatcher3D& dispatcher) {
    dispatcher.dispatch(*this);
}

void GeometryNode::updateDataset() {
    m_dataset->fetch();
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