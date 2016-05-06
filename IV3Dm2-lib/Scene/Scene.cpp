//  Created by David McCann on 5/5/16.
//  Copyright © 2016 Scientific Computing and Imaging Institute. All rights reserved.
//

#include "Scene.h"

#include "SceneParser.h"

Scene::Scene(SceneMetadata metadata, std::unique_ptr<SceneNode> sceneRoot)
    : m_metadata(std::move(metadata))
    , m_sceneRoot(std::move(sceneRoot)) {}

Scene Scene::fromJson(const JsonCpp::Value& root) {
    return SceneParser::parseScene(root);
}

SceneMetadata Scene::metadata() const noexcept {
    return m_metadata;
}

const SceneNode& Scene::rootNode() const noexcept {
    return *m_sceneRoot;
}
