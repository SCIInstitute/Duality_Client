//  Created by David McCann on 5/5/16.
//  Copyright © 2016 Scientific Computing and Imaging Institute. All rights reserved.
//

#pragma once

#include "Scene/Scene.h"
#include "Scene/SceneMetadata.h"

#include <vector>
#include <memory>

class ServerAdapter;

class SceneLoader {
public:
    SceneLoader(std::unique_ptr<ServerAdapter> server);
    
    std::vector<SceneMetadata> listMetadata() const;
    bool loadScene(const std::string& name);
    Scene* activeScene() const;
    
private:
    std::unique_ptr<ServerAdapter> m_server;
    std::unique_ptr<Scene> m_scene;
};