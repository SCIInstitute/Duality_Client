//  Created by David McCann on 5/10/16.
//  Copyright © 2016 Scientific Computing and Imaging Institute. All rights reserved.
//

#include "duality/SceneLoader.h"

#include "src/duality/Communication.h"
#include "src/duality/SceneController3DImpl.h"
#include "src/duality/SceneParser.h"

#include "IVDA/Vectors.h"
#include "duality/Error.h"

#include "jsoncpp/json.h"

#include <algorithm>
#include <cassert>

using namespace IVDA;

class SceneLoaderImpl {
public:
    SceneLoaderImpl(const mocca::net::Endpoint& endpoint);

    std::vector<SceneMetadata> listMetadata() const;
    bool loadScene(const std::string& name);

    std::weak_ptr<SceneController3D> sceneController3D(const ScreenInfo& screenInfo);

private:
    void createSceneController3D(const ScreenInfo& screenInfo);

private:
    std::shared_ptr<LazyRpcClient> m_rpc;
    std::unique_ptr<Scene> m_scene;
    std::shared_ptr<SceneController3D> m_sceneController3D;
};

SceneLoaderImpl::SceneLoaderImpl(const mocca::net::Endpoint& endpoint)
    : m_rpc(std::make_shared<LazyRpcClient>(endpoint)) {}

std::vector<SceneMetadata> SceneLoaderImpl::listMetadata() const {
    m_rpc->send("listScenes", JsonCpp::Value());
    auto root = m_rpc->receive().first;
    std::vector<SceneMetadata> result;
    for (auto it = root.begin(); it != root.end(); ++it) {
        result.push_back(SceneParser::parseMetadata(*it));
    }
    return result;
}

bool SceneLoaderImpl::loadScene(const std::string& name) {
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

std::weak_ptr<SceneController3D> SceneLoaderImpl::sceneController3D(const ScreenInfo& screenInfo) {
    if (m_sceneController3D == nullptr) {
        createSceneController3D(screenInfo);
    }
    return m_sceneController3D;
}

void SceneLoaderImpl::createSceneController3D(const ScreenInfo& screenInfo) {
    RenderParameters3D initialParameters(Vec3f(0.0f, 0.0f, -3.0f), Mat4f());
    BoundingBox boundingBox = duality::calculateSceneBoundingBox(*m_scene);
    auto mvp = std::make_unique<MVP3D>(screenInfo, boundingBox);
    auto renderDispatcher = std::make_unique<RenderDispatcher3D>(screenInfo);
    auto impl = std::make_unique<SceneController3DImpl>(*m_scene, initialParameters, std::move(renderDispatcher), std::move(mvp));
    m_sceneController3D = std::make_shared<SceneController3D>(std::move(impl));
}

SceneLoader::SceneLoader(const mocca::net::Endpoint& endpoint)
    : m_impl(std::make_unique<SceneLoaderImpl>(endpoint)) {}

SceneLoader::~SceneLoader() = default;

std::vector<SceneMetadata> SceneLoader::listMetadata() const {
    return m_impl->listMetadata();
}

bool SceneLoader::loadScene(const std::string& name) {
    return m_impl->loadScene(name);
}

std::weak_ptr<SceneController3D> SceneLoader::sceneController3D(const ScreenInfo& screenInfo) {
    return m_impl->sceneController3D(screenInfo);
}
