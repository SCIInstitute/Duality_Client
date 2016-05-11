//  Created by David McCann on 5/5/16.
//  Copyright © 2016 Scientific Computing and Imaging Institute. All rights reserved.
//

#include "SceneParser.h"

#include "Common/Error.h"
#include "Scene/DownloadProvider.h"
#include "Scene/GeometryNode.h"
#include "Scene/ServerAdapter.h"

using namespace IVDA;

SceneParser::SceneParser(const JsonCpp::Value& root, const ServerAdapter* serverAdapter)
    : m_root(root)
    , m_serverAdapter(serverAdapter) {}

SceneMetadata SceneParser::parseMetadata() {
    std::string name = m_root["metadata"]["name"].asString();
    std::string description = m_root["metadata"]["description"].asString();
    return SceneMetadata(std::move(name), std::move(description));
}

std::unique_ptr<Scene> SceneParser::parseScene() {
    auto metadata = parseMetadata();
    auto node = std::make_unique<Scene>(std::move(metadata));
    auto sceneJson = m_root["scene"];
    for (auto it = sceneJson.begin(); it != sceneJson.end(); ++it) {
        node->addNode(parseNode(*it));
    }
    return node;
}

std::unique_ptr<SceneNode> SceneParser::parseNode(const JsonCpp::Value& node) {
    std::string type = node["type"].asString();
    if (type == "geo") {
        return parseGeometry(node);
    }
    throw Error("Invalid node type: " + type, __FILE__, __LINE__);
}

std::unique_ptr<SceneNode> SceneParser::parseGeometry(const JsonCpp::Value& node) {
    auto provider = parseDataSource(node["source"]);
    std::vector<Mat4f> transforms;
    if (node.isMember("transforms")) {
        transforms = parseMatrices(node["transforms"]);
    }
    return std::make_unique<GeometryNode>(std::move(provider), std::move(transforms));
}

std::unique_ptr<DataProvider> SceneParser::parseDataSource(const JsonCpp::Value& node) {
    return std::make_unique<DownloadProvider>(m_serverAdapter, node["path"].asString());
}

Mat4f SceneParser::parseMatrix(const JsonCpp::Value& node) {
    if (node.size() != 16) {
        THROW_ERROR("Invalid matrix definition in JSON");
    }
    return Mat4f(node[0].asFloat(), node[1].asFloat(), node[2].asFloat(), node[3].asFloat(), node[4].asFloat(), node[5].asFloat(),
                 node[6].asFloat(), node[7].asFloat(), node[8].asFloat(), node[9].asFloat(), node[10].asFloat(), node[11].asFloat(),
                 node[12].asFloat(), node[13].asFloat(), node[14].asFloat(), node[15].asFloat());
}

std::vector<IVDA::Mat4f> SceneParser::parseMatrices(const JsonCpp::Value& node) {
    std::vector<Mat4f> result;
    for (auto it = node.begin(); it != node.end(); ++it) {
        result.push_back(parseMatrix(*it));
    }
    return result;
}