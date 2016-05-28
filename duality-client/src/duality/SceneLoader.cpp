//  Created by David McCann on 5/10/16.
//  Copyright © 2016 Scientific Computing and Imaging Institute. All rights reserved.
//

#include "duality/SceneLoader.h"

#include "src/duality/SceneParser.h"
#include "src/duality/Communication.h"

#include "duality/Error.h"

#include "jsoncpp/json.h"

#include <algorithm>
#include <cassert>

class SceneLoaderImpl {
public:
    SceneLoaderImpl(const mocca::net::Endpoint& endpoint)
        : m_rpc(std::make_shared<LazyRpcClient>(endpoint)) {}

    std::vector<SceneMetadata> listMetadata() const {
        m_rpc->send("listScenes", JsonCpp::Value());
        auto root = m_rpc->receive().first;
        std::vector<SceneMetadata> result;
        for (auto it = root.begin(); it != root.end(); ++it) {
            result.push_back(SceneParser::parseMetadata(*it));
        }
        return result;
    }

    bool loadScene(const std::string& name) {
        if (m_scene != nullptr && m_scene->metadata().name() == name)
            return false;
        m_rpc->send("listScenes", JsonCpp::Value());
        auto root = m_rpc->receive().first;
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

private:
    std::shared_ptr<LazyRpcClient> m_rpc;
    std::unique_ptr<Scene> m_scene;
};

SceneLoader::SceneLoader(const mocca::net::Endpoint& endpoint)
    : m_impl(std::make_unique<SceneLoaderImpl>(endpoint)) {}

SceneLoader::~SceneLoader() = default;

std::vector<SceneMetadata> SceneLoader::listMetadata() const {
    return m_impl->listMetadata();
}

bool SceneLoader::loadScene(const std::string& name) {
    return m_impl->loadScene(name);
}