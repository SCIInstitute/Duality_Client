//  Created by David McCann on 5/5/16.
//  Copyright © 2016 Scientific Computing and Imaging Institute. All rights reserved.
//

#pragma once

#include <string>

#include "jsoncpp/json.h"

class SceneDefinition {
public:
    SceneDefinition(const std::string& name);
    static SceneDefinition fromJson(const JsonCpp::Value& root);
    
    std::string name() const;
    
private:
    std::string m_name;
};