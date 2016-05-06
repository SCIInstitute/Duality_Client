//  Created by David McCann on 5/5/16.
//  Copyright © 2016 Scientific Computing and Imaging Institute. All rights reserved.
//

#include "SceneParser.h"

#include "Common/Error.h"
#include "Dataset.h"
#include "DownloadNode.h"
#include "GroupNode.h"

SceneMetadata SceneParser::parseMetadata(const JsonCpp::Value& root) {
    std::string name = root["name"].asString();
    std::string description = root["description"].asString();
    return SceneMetadata(std::move(name), std::move(description));
}

Scene SceneParser::parseScene(const JsonCpp::Value& root) {
    auto metadata = parseMetadata(root["metadata"]);
    auto sceneRoot = parseNode(root["scene"]);
    return Scene(std::move(metadata), std::move(sceneRoot));
}

std::unique_ptr<SceneNode> SceneParser::parseNode(const JsonCpp::Value& node) {
    std::string type = node["type"].asString();
    if (type == "download") {
        return parseDownloadNode(node);
    } else if (type == "group") {
        return parseGroupNode(node);
    }
    throw Error("Invalid node type: " + type, __FILE__, __LINE__);
}

std::unique_ptr<SceneNode> SceneParser::parseDownloadNode(const JsonCpp::Value& node) {
    std::string path = node["path"].asString();
    return std::make_unique<DownloadNode>(path);
}

std::unique_ptr<SceneNode> SceneParser::parseGroupNode(const JsonCpp::Value& node) {
    auto result = std::make_unique<GroupNode>();
    const auto& children = node["children"];
    for (auto it = children.begin(); it != children.end(); ++it) {
        result->addChild(parseNode(*it));
    }
    return std::move(result);
}