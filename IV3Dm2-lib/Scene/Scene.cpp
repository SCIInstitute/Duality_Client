//  Created by David McCann on 5/5/16.
//  Copyright © 2016 Scientific Computing and Imaging Institute. All rights reserved.
//

#include "Scene.h"

#include "SceneParser.h"
#include "Render/RenderDispatcher.h"

Scene::Scene(SceneMetadata metadata, std::unique_ptr<SceneNode> sceneRoot)
    : m_metadata(std::move(metadata))
    , m_sceneRoot(std::move(sceneRoot)) {}

SceneMetadata Scene::metadata() const noexcept {
    return m_metadata;
}

const SceneNode& Scene::rootNode() const noexcept {
    return *m_sceneRoot;
}

void Scene::updateDatasets() {
    m_sceneRoot->updateDataset();
}

void Scene::render(RenderDispatcher& dispatcher) const {
    m_sceneRoot->accept(dispatcher);
}
