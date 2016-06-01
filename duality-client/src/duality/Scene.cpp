#include "src/duality/Scene.h"

#include "IVDA/Vectors.h"
#include "src/duality/SceneParser.h"
#include "src/duality/View.h"

using namespace IVDA;

Scene::Scene(SceneMetadata metadata)
    : m_metadata(std::move(metadata)) {}

SceneMetadata Scene::metadata() const {
    return m_metadata;
}

void Scene::addNode(SceneNode node) {
    m_nodes.push_back(std::move(node));
}

const std::vector<SceneNode>& Scene::nodes() const {
    return m_nodes;
}

std::vector<SceneNode>& Scene::nodes() {
    return m_nodes;
}

void Scene::dispatch(DatasetDispatcher& dispatcher, View view) const {
    for (auto& node : m_nodes) {
        if (node.dataset()->isVisibleInView(view)) {
            node.dispatch(dispatcher);
        }
    }
}

void Scene::updateDatasets() {
    for (auto& node : m_nodes) {
        node.updateDataset();
    }
}

VariableInfoMap Scene::variableInfoMap(View view) {
    VariableInfoMap result;
    for (auto& node : m_nodes) {
        if (node.dataset()->isVisibleInView(view)) {
            result[node.name()].floatInfos = node.dataProvider()->floatVariableInfos();
            result[node.name()].enumInfos = node.dataProvider()->enumVariableInfos();
        }
    }
    return result;
}

void Scene::setVariable(const std::string& objectName, const std::string& variableName, float value) {
    auto it = std::find_if(begin(m_nodes), end(m_nodes), [&objectName](const SceneNode& node) { return node.name() == objectName; });
    assert(it != end(m_nodes));
    it->setVariable(variableName, value);
}

void Scene::setVariable(const std::string& objectName, const std::string& variableName, const std::string& value) {
    auto it = std::find_if(begin(m_nodes), end(m_nodes), [&objectName](const SceneNode& node) { return node.name() == objectName; });
    assert(it != end(m_nodes));
    it->setVariable(variableName, value);
}

BoundingBox duality::calculateSceneBoundingBox(const Scene& scene, View view) {
    BoundingBoxCalculator bbCalc;
    scene.dispatch(bbCalc, view);
    return bbCalc.boundingBox();
}
