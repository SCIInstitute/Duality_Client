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
    auto infos1 = m_dataset->floatVariableInfos();
    auto infos2 = m_tf->floatVariableInfos();
    infos1.insert(end(infos1), begin(infos2), end(infos2));
    return infos1;
}

std::vector<EnumVariableInfo> VolumeNode::enumVariableInfos() const {
    auto infos1 = m_dataset->enumVariableInfos();
    auto infos2 = m_tf->enumVariableInfos();
    infos1.insert(end(infos1), begin(infos2), end(infos2));
    return infos1;
}

void VolumeNode::setVariable(const std::string& variableName, float value) {
    if (m_dataset->hasFloatVariable(variableName)) {
        m_dataset->setVariable(variableName, value);
    } else {
        assert(m_tf->hasFloatVariable(variableName));
        m_tf->setVariable(variableName, value);
    }
}

void VolumeNode::setVariable(const std::string& variableName, const std::string& value) {
    if (m_dataset->hasEnumVariable(variableName)) {
        m_dataset->setVariable(variableName, value);
    } else {
        assert(m_tf->hasEnumVariable(variableName));
        m_tf->setVariable(variableName, value);
    }
}

const VolumeDataset& VolumeNode::dataset() const {
    return *m_dataset;
}

const TransferFunction& VolumeNode::transferFunction() const {
    return *m_tf;
}
