#include "duality/SceneLoader.h"

#include "src/duality/Communication.h"
#include "src/duality/RenderDispatcher2D.h"
#include "src/duality/RenderDispatcher3D.h"
#include "src/duality/SceneController2DImpl.h"
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
    bool isSceneLoaded() const;

    std::weak_ptr<SceneController2D> sceneController2D();
    std::weak_ptr<SceneController3D> sceneController3D();

private:
    void createSceneController2D();
    void createSceneController3D();

private:
    std::shared_ptr<LazyRpcClient> m_rpc;
    std::shared_ptr<GLFrameBufferObject> m_resultFbo;
    std::unique_ptr<Scene> m_scene;
    std::shared_ptr<SceneController2D> m_sceneController2D;
    std::shared_ptr<SceneController3D> m_sceneController3D;
};

SceneLoaderImpl::SceneLoaderImpl(const mocca::net::Endpoint& endpoint)
    : m_rpc(std::make_shared<LazyRpcClient>(endpoint))
    , m_resultFbo(std::make_shared<GLFrameBufferObject>()) {}

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
            m_sceneController2D = nullptr;
            m_sceneController3D = nullptr;
            return true;
        }
    }
    throw Error("Scene named '" + name + "' does not exist", __FILE__, __LINE__);
}

bool SceneLoaderImpl::isSceneLoaded() const {
    return m_scene != nullptr;
}

std::weak_ptr<SceneController2D> SceneLoaderImpl::sceneController2D() {
    if (m_sceneController2D == nullptr) {
        createSceneController2D();
    }
    return m_sceneController2D;
}

std::weak_ptr<SceneController3D> SceneLoaderImpl::sceneController3D() {
    if (m_sceneController3D == nullptr) {
        createSceneController3D();
    }
    return m_sceneController3D;
}

void SceneLoaderImpl::createSceneController2D() {
    m_scene->updateDatasets();
    RenderParameters2D initialParameters(Vec2f(0.0f, 0.0f), 0.0f, 1.0f, CoordinateAxis::X_Axis);
    // BoundingBox boundingBox = duality::calculateSceneBoundingBox(*m_scene);
    // auto mvp = std::make_unique<MVP2D>(screenInfo, boundingBox);
    // auto renderDispatcher = std::make_unique<RenderDispatcher2D>(screenInfo);
    auto impl = std::make_unique<SceneController2DImpl>(*m_scene, initialParameters, m_resultFbo);
    m_sceneController2D = std::make_shared<SceneController2D>(std::move(impl));
}

void SceneLoaderImpl::createSceneController3D() {
    m_scene->updateDatasets();
    RenderParameters3D initialParameters(Vec3f(0.0f, 0.0f, -3.0f), Mat4f());
    // BoundingBox boundingBox = duality::calculateSceneBoundingBox(*m_scene);
    // auto mvp = std::make_unique<MVP3D>(screenInfo, boundingBox);
    // auto renderDispatcher = std::make_unique<RenderDispatcher3D>(screenInfo);
    auto impl = std::make_unique<SceneController3DImpl>(*m_scene, initialParameters, m_resultFbo);
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

bool SceneLoader::isSceneLoaded() const {
    return m_impl->isSceneLoaded();
}

std::weak_ptr<SceneController2D> SceneLoader::sceneController2D() {
    return m_impl->sceneController2D();
}

std::weak_ptr<SceneController3D> SceneLoader::sceneController3D() {
    return m_impl->sceneController3D();
}
