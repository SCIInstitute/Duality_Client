//  Created by David McCann on 5/5/16.
//  Copyright © 2016 Scientific Computing and Imaging Institute. All rights reserved.
//

#include "SceneParser.h"

#include "Common/Error.h"
#include "Communication/DownloadProvider.h"
#include "Communication/ServerAdapter.h"
#include "GeometryNode.h"
#include "GroupNode.h"
#include "Scene.h"

SceneParser::SceneParser(const JsonCpp::Value& root, const ServerAdapter& serverAdapter)
    : m_root(root)
    , m_serverAdapter(serverAdapter) {}

SceneMetadata SceneParser::parseMetadata() {
    std::string name = m_root["metadata"]["name"].asString();
    std::string description = m_root["metadata"]["description"].asString();
    return SceneMetadata(std::move(name), std::move(description));
}

std::unique_ptr<Scene> SceneParser::parseScene() {
    auto metadata = parseMetadata();
    auto sceneRoot = parseNode(m_root["scene"]);
    return std::make_unique<Scene>(std::move(metadata), std::move(sceneRoot));
}

std::unique_ptr<SceneNode> SceneParser::parseNode(const JsonCpp::Value& node) {
    std::string type = node["type"].asString();
    if (type == "geo") {
        return parseGeometry(node);
    } else if (type == "group") {
        return parseGroupNode(node);
    }
    throw Error("Invalid node type: " + type, __FILE__, __LINE__);
}

std::unique_ptr<SceneNode> SceneParser::parseGeometry(const JsonCpp::Value& node) {
    auto provider = parseDataSource(node["source"]);
    return std::make_unique<GeometryNode>(std::move(provider));
}

std::unique_ptr<SceneNode> SceneParser::parseGroupNode(const JsonCpp::Value& node) {
    auto result = std::make_unique<GroupNode>(nullptr);
    const auto& children = node["children"];
    for (auto it = children.begin(); it != children.end(); ++it) {
        result->addChild(parseNode(*it));
    }
    return std::move(result);
}

std::unique_ptr<DataProvider> SceneParser::parseDataSource(const JsonCpp::Value& node) {
    return std::make_unique<DownloadProvider>(m_serverAdapter, node["path"].asString());
}