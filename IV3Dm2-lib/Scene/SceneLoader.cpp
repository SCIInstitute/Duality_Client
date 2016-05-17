//  Created by David McCann on 5/10/16.
//  Copyright © 2016 Scientific Computing and Imaging Institute. All rights reserved.
//

#include "SceneLoader.h"

#include "Common/Error.h"
#include "Scene/SceneParser.h"
#include "Scene/ServerAdapter.h"

#include <algorithm>
#include <cassert>

SceneLoader::SceneLoader(std::unique_ptr<ServerAdapter> server)
    : m_server(std::move(server)) {
    assert(m_server != nullptr);
}

std::vector<SceneMetadata> SceneLoader::listMetadata() const {
    auto root = m_server->scenes();
    std::vector<SceneMetadata> result;
    for (auto it = root.begin(); it != root.end(); ++it) {
        SceneParser parser(*it, m_server.get());
        result.push_back(parser.parseMetadata());
    }
    return result;
}

bool SceneLoader::loadScene(const std::string& name) {
    if (m_scene != nullptr && m_scene->metadata().name() == name) return false;
    auto root = m_server->scenes();
    for (auto it = root.begin(); it != root.end(); ++it) {
        SceneParser parser(*it, m_server.get());
        auto metadata = parser.parseMetadata();
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