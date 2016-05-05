//  Created by David McCann on 5/5/16.
//  Copyright © 2016 Scientific Computing and Imaging Institute. All rights reserved.
//

#pragma once

#include "SceneNode.h"

#include "jsoncpp/json.h"

#include <string>
#include <memory>

class SceneDefinition {
public:
    SceneDefinition(std::string name, std::unique_ptr<SceneNode> sceneRoot);
    static SceneDefinition fromJson(const JsonCpp::Value& root);
    
    std::string name() const noexcept;
    
private:
    std::string m_name;
    std::unique_ptr<SceneNode> m_sceneRoot;
};