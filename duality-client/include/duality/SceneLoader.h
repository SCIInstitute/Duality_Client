//  Created by David McCann on 5/5/16.
//  Copyright © 2016 Scientific Computing and Imaging Institute. All rights reserved.
//

#pragma once

#include "duality/SceneController2D.h"
#include "duality/SceneController3D.h"
#include "duality/SceneMetadata.h"
#include "duality/ScreenInfo.h"

#include "mocca/net/Endpoint.h"

#include <memory>
#include <vector>

class SceneLoaderImpl;

class SceneLoader {
public:
    SceneLoader(const mocca::net::Endpoint& endpoint);
    ~SceneLoader();

    std::vector<SceneMetadata> listMetadata() const;
    bool loadScene(const std::string& name);
    bool isSceneLoaded() const;
    std::weak_ptr<SceneController2D> sceneController2D();
    std::weak_ptr<SceneController3D> sceneController3D();

private:
    std::unique_ptr<SceneLoaderImpl> m_impl;
};