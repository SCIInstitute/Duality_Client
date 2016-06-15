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

std::vector<FloatVariableInfo> GeometryNode::floatVariableInfos() const {
    return m_dataset->floatVariableInfos();
}

std::vector<EnumVariableInfo> GeometryNode::enumVariableInfos() const {
    return m_dataset->enumVariableInfos();
}

void GeometryNode::setVariable(const std::string& variableName, float value) {
    m_dataset->setVariable(variableName, value);
}

void GeometryNode::setVariable(const std::string& variableName, const std::string& value) {
    m_dataset->setVariable(variableName, value);
}

const GeometryDataset& GeometryNode::dataset() const {
    return *m_dataset;
}
