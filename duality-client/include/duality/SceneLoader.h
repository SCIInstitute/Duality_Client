#pragma once

#include "duality/SceneController2D.h"
#include "duality/SceneController3D.h"
#include "duality/SceneMetadata.h"
#include "duality/ScreenInfo.h"

#include "mocca/fs/Path.h"
#include "mocca/net/Endpoint.h"

#include <memory>
#include <vector>

class SceneLoaderImpl;

class SceneLoader {
public:
    SceneLoader(const mocca::net::Endpoint& endpoint, const mocca::fs::Path& cacheDir);
    ~SceneLoader();

    void updateEndpoint(const mocca::net::Endpoint& endpoint);
    void clearCache();
    void setCachingEnabled(bool enabled);
    
    std::vector<SceneMetadata> listMetadata() const;
    void loadScene(const std::string& name);
    bool isSceneLoaded() const;
    SceneMetadata currentMetadata() const;
    
    std::weak_ptr<SceneController2D> sceneController2D();
    std::weak_ptr<SceneController3D> sceneController3D();

private:
    std::unique_ptr<SceneLoaderImpl> m_impl;
};