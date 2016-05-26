//  Created by David McCann on 5/5/16.
//  Copyright © 2016 Scientific Computing and Imaging Institute. All rights reserved.
//

#include "duality/Scene.h"

#include "IVDA/Vectors.h"
#include "duality/BoundingBoxCalculator.h"
#include "duality/SceneParser.h"

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

void Scene::dispatch(DatasetDispatcher& dispatcher) const {
    for (auto& node : m_nodes) {
        node.dispatch(dispatcher);
    }
}

void Scene::updateDatasets() {
    for (auto& node : m_nodes) {
        node.updateDataset();
    }
}

Scene::VariableMap Scene::variableMap() {
    VariableMap result;
    for (auto& node : m_nodes) {
        result[node.name()].floatInfos = node.dataProvider()->floatVariableInfos();
        result[node.name()].enumInfos = node.dataProvider()->enumVariableInfos();
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