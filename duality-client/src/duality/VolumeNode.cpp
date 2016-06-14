#include "src/duality/VolumeNode.h"

VolumeNode::VolumeNode(const std::string& name, Visibility visibility, std::unique_ptr<VolumeDataset> dataset)
    : SceneNode(name, visibility)
    , m_dataset(std::move(dataset)) {}

void VolumeNode::updateDataset() {
    m_dataset->fetch();
}

void VolumeNode::dispatch(DatasetDispatcher& dispatcher) const {
    m_dataset->accept(dispatcher);
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
