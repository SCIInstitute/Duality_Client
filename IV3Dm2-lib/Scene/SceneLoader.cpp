//  Created by David McCann on 5/10/16.
//  Copyright © 2016 Scientific Computing and Imaging Institute. All rights reserved.
//

#include "SceneLoader.h"

#include "Common/Error.h"
#include "Scene/SceneParser.h"

#include <algorithm>
#include <cassert>


SceneLoader::SceneLoader(const mocca::net::Endpoint& endpoint)
    : m_rpc(std::make_shared<LazyRpcClient>(endpoint)) {}

std::vector<SceneMetadata> SceneLoader::listMetadata() const {
    auto root = fetchScenes();
    std::vector<SceneMetadata> result;
    for (auto it = root.begin(); it != root.end(); ++it) {
        result.push_back(SceneParser::parseMetadata(*it));
    }
    return result;
}

bool SceneLoader::loadScene(const std::string& name) {
    if (m_scene != nullptr && m_scene->metadata().name() == name)
        return false;
    auto root = fetchScenes();
    for (auto it = root.begin(); it != root.end(); ++it) {
        SceneParser parser(*it, m_rpc);
        auto metadata = SceneParser::parseMetadata(*it);
        if (metadata.name() == name) {
            m_scene = parser.parseScene();
            return true;
        }
    }
    throw Error("Scene named '" + name + "' does not exist", __FILE__, __LINE__);
}

Scene* SceneLoader::activeScene() const {
    return m_scene.get();
}

JsonCpp::Value SceneLoader::fetchScenes() const {
    m_rpc->send("listScenes", JsonCpp::Value());
    return m_rpc->receive().first;
}
