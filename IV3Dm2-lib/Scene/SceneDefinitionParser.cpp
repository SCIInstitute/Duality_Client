//  Created by David McCann on 5/5/16.
//  Copyright © 2016 Scientific Computing and Imaging Institute. All rights reserved.
//

#include "SceneDefinitionParser.h"

#include "Common/Error.h"
#include "DownloadNode.h"
#include "GroupNode.h"

SceneDefinition SceneDefinitionParser::parse(const JsonCpp::Value& root) {
    std::string name = root["name"].asString();
    auto sceneRoot = parseNode(root["scene"]);
    return SceneDefinition(name, std::move(sceneRoot));
}

std::unique_ptr<SceneNode> SceneDefinitionParser::parseNode(const JsonCpp::Value& node) {
    std::string type = node["type"].asString();
    if (type == "download") {
        return parseDownloadNode(node);
    } else if (type == "group") {
        return parseGroupNode(node);
    }
    throw Error("Invalid node type: " + type, __FILE__, __LINE__);
}

std::unique_ptr<SceneNode> SceneDefinitionParser::parseDownloadNode(const JsonCpp::Value& node) {
    std::string path = node["path"].asString();
    return std::make_unique<DownloadNode>(path);
}

std::unique_ptr<SceneNode> SceneDefinitionParser::parseGroupNode(const JsonCpp::Value& node) {
    auto result = std::make_unique<GroupNode>();
    const auto& children = node["children"];
    for (auto it = children.begin(); it != children.end(); ++it) {
        result->addChild(parseNode(*it));
    }
    return std::move(result);
}