//  Created by David McCann on 5/5/16.
//  Copyright © 2016 Scientific Computing and Imaging Institute. All rights reserved.
//

#pragma once

#include "Scene/Scene.h"
#include "Scene/SceneMetadata.h"
#include "Scene/Communication.h"

#include <vector>
#include <memory>

class ServerAdapter;

class SceneLoader {
public:
    SceneLoader(const mocca::net::Endpoint& endpoint);
    
    std::vector<SceneMetadata> listMetadata() const;
    bool loadScene(const std::string& name);
    Scene* activeScene() const;
    
private:
    JsonCpp::Value fetchScenes() const;

private:
    std::shared_ptr<LazyRpcClient> m_rpc;
    std::unique_ptr<Scene> m_scene;
};