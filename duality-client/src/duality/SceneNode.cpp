#include "duality/SceneNode.h"

SceneNode::SceneNode(const std::string& name, std::unique_ptr<DataProvider> provider, std::unique_ptr<Dataset> dataset)
    : m_name(name)
    , m_provider(std::move(provider))
    , m_dataset(std::move(dataset)) {}

void SceneNode::updateDataset() {
    auto data = m_provider->fetch();
    if (data != nullptr) {
        m_dataset->load(data);
    }
}

void SceneNode::dispatch(DatasetDispatcher& dispatcher) const {
    m_dataset->accept(dispatcher);
}

std::string SceneNode::name() const {
    return m_name;
}

void SceneNode::setVariable(const std::string& variableName, float value) {
    m_provider->setVariable(variableName, value);
}

void SceneNode::setVariable(const std::string& variableName, const std::string& value) {
    m_provider->setVariable(variableName, value);
}

const DataProvider* SceneNode::dataProvider() const {
    return m_provider.get();
}

DataProvider* SceneNode::dataProvider() {
    return m_provider.get();
}

const Dataset* SceneNode::dataset() const {
    return m_dataset.get();
}
