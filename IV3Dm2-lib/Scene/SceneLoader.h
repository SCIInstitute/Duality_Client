//  Created by David McCann on 5/5/16.
//  Copyright © 2016 Scientific Computing and Imaging Institute. All rights reserved.
//

#pragma once

#include "Scene/Scene.h"
#include "Scene/SceneMetadata.h"

#include <vector>

class ServerAdapter;

class SceneLoader {
public:
    SceneLoader(const ServerAdapter* server);
    
    const std::vector<SceneMetadata>& listMetadata() const;
    std::unique_ptr<Scene> getScene(const std::string& name) const;
    
private:
    const ServerAdapter* m_server;
    JsonCpp::Value m_root;
    std::vector<SceneMetadata> m_metadata;
};