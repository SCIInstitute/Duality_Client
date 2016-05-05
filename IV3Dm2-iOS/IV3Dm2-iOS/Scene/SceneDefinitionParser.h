//  Created by David McCann on 5/5/16.
//  Copyright © 2016 Scientific Computing and Imaging Institute. All rights reserved.
//

#pragma once

#include "SceneDefinition.h"

#include "jsoncpp/json.h"

class SceneDefinitionParser {
public:
    static SceneDefinition parse(const JsonCpp::Value& root);
    
private:
    static std::unique_ptr<SceneNode> parseNode(const JsonCpp::Value& node);
    static std::unique_ptr<SceneNode> parseDownloadNode(const JsonCpp::Value& node);
    static std::unique_ptr<SceneNode> parseGroupNode(const JsonCpp::Value& node);
};