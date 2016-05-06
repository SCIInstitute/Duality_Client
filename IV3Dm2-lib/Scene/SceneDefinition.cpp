//  Created by David McCann on 5/5/16.
//  Copyright © 2016 Scientific Computing and Imaging Institute. All rights reserved.
//

#include "SceneDefinition.h"

#include "SceneDefinitionParser.h"

SceneDefinition::SceneDefinition(std::string name, std::unique_ptr<SceneNode> sceneRoot)
    : m_name(std::move(name))
    , m_sceneRoot(std::move(sceneRoot)) {}

SceneDefinition SceneDefinition::fromJson(const JsonCpp::Value& root) {
    return SceneDefinitionParser::parse(root);
}

std::string SceneDefinition::name() const noexcept {
    return m_name;
}

const SceneNode& SceneDefinition::rootNode() const noexcept {
    return *m_sceneRoot;
}
