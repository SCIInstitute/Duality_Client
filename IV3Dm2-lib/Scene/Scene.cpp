//  Created by David McCann on 5/5/16.
//  Copyright © 2016 Scientific Computing and Imaging Institute. All rights reserved.
//

#include "Scene.h"

#include "Scene/RenderDispatcher.h"
#include "Scene/SceneParser.h"

Scene::Scene(SceneMetadata metadata)
    : m_metadata(std::move(metadata)) {}

SceneMetadata Scene::metadata() const noexcept {
    return m_metadata;
}

void Scene::addNode(std::unique_ptr<SceneNode> node) {
    m_nodes.push_back(std::move(node));
}

const std::vector<std::unique_ptr<SceneNode>>& Scene::nodes() const noexcept {
    return m_nodes;
}

void Scene::updateDatasets() {
    for (auto& node : m_nodes) {
        node->updateDataset();
    }
}

void Scene::render(RenderDispatcher& dispatcher) const {
    for (auto& node : m_nodes) {
        node->accept(dispatcher);
    }
}
