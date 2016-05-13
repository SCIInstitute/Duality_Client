#include "Scene/SceneNode.h"

SceneNode::SceneNode(std::unique_ptr<DataProvider> provider, std::unique_ptr<Dataset> dataset)
    : m_provider(std::move(provider))
    , m_dataset(std::move(dataset)) {}

void SceneNode::dispatch(DatasetDispatcher& dispatcher) {
    m_dataset->accept(dispatcher);
}

void SceneNode::dispatch(DataProviderDispatcher& dispatcher) {
    m_provider->accept(dispatcher);
}

void SceneNode::loadDataset(std::shared_ptr<std::vector<uint8_t>> data) {
    m_dataset->load(data);
}

const DataProvider* SceneNode::dataProvider() const {
    return m_provider.get();
}

const Dataset* SceneNode::dataset() const {
    return m_dataset.get();
}
