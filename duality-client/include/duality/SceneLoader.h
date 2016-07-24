#pragma once

#include "duality/SceneController2D.h"
#include "duality/SceneController3D.h"
#include "duality/SceneMetadata.h"
#include "duality/ScreenInfo.h"
#include "duality/Settings.h"

#include "mocca/fs/Path.h"
#include "mocca/net/Endpoint.h"

#include <memory>
#include <vector>
#include <functional>

class SceneLoaderImpl;

class SceneLoader {
public:
    SceneLoader(const mocca::fs::Path& cacheDir, std::shared_ptr<Settings> settings);
    ~SceneLoader();
    
    std::shared_ptr<Settings> settings();    
    void updateEndpoint();
    void clearCache();
    
    std::vector<SceneMetadata> listMetadata() const;
    void loadScene(const std::string& name);
    bool isSceneLoaded() const;
    SceneMetadata metadata() const;
    std::string webViewURL() const;
    
    std::shared_ptr<SceneController2D> sceneController2D(std::function<void(int, int, const std::string&)> updateDatasetCallback);
    std::shared_ptr<SceneController3D> sceneController3D(std::function<void(int, int, const std::string&)> updateDatasetCallback);

private:
    std::unique_ptr<SceneLoaderImpl> m_impl;
};