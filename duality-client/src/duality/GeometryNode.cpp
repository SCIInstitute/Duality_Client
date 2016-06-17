#include "src/duality/GeometryNode.h"

GeometryNode::GeometryNode(const std::string& name, Visibility visibility, std::unique_ptr<GeometryDataset> dataset)
    : SceneNode(name, visibility)
    , m_dataset(std::move(dataset)) {}

void GeometryNode::accept(NodeDispatcher& dispatcher) {
    dispatcher.dispatch(*this);
}

void GeometryNode::updateDataset() {
    m_dataset->fetch();
}

const GeometryDataset& GeometryNode::dataset() const {
    return *m_dataset;
}
