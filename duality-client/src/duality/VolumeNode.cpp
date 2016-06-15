#include "src/duality/VolumeNode.h"

VolumeNode::VolumeNode(const std::string& name, Visibility visibility, std::unique_ptr<VolumeDataset> dataset,
                       std::unique_ptr<TransferFunction> tf)
    : SceneNode(name, visibility)
    , m_dataset(std::move(dataset))
    , m_tf(std::move(tf)) {}

void VolumeNode::accept(NodeDispatcher& dispatcher) {
    dispatcher.dispatch(*this);
}

void VolumeNode::updateDataset() {
    m_dataset->fetch();
    m_tf->fetch();
}

std::vector<FloatVariableInfo> VolumeNode::floatVariableInfos() const {
    return m_dataset->floatVariableInfos();
}

std::vector<EnumVariableInfo> VolumeNode::enumVariableInfos() const {
    return m_dataset->enumVariableInfos();
}

void VolumeNode::setVariable(const std::string& variableName, float value) {
    m_dataset->setVariable(variableName, value);
}

void VolumeNode::setVariable(const std::string& variableName, const std::string& value) {
    m_dataset->setVariable(variableName, value);
}

const VolumeDataset& VolumeNode::dataset() const {
    return *m_dataset;
}

const TransferFunction& VolumeNode::transferFunction() const {
    return *m_tf;
}
