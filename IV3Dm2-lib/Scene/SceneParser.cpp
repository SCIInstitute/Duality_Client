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
    SceneNode::MatrixTriple transforms = {nullptr, nullptr, nullptr};
    if (node.isMember("transforms")) {
        transforms = parseMatrices(node["transforms"]);
    }
    return std::make_unique<GeometryNode>(std::move(provider), std::move(transforms));
}

std::unique_ptr<SceneNode> SceneParser::parseGroupNode(const JsonCpp::Value& node) {
    SceneNode::MatrixTriple transforms = {nullptr, nullptr, nullptr};
    if (node.isMember("transforms")) {
        transforms = parseMatrices(node["transforms"]);
    }
    auto result = std::make_unique<GroupNode>(nullptr, std::move(transforms));
    const auto& children = node["children"];
    for (auto it = children.begin(); it != children.end(); ++it) {
        result->addChild(parseNode(*it));
    }
    return std::move(result);
}

std::unique_ptr<DataProvider> SceneParser::parseDataSource(const JsonCpp::Value& node) {
    return std::make_unique<DownloadProvider>(m_serverAdapter, node["path"].asString());
}

std::unique_ptr<GLMatrix> SceneParser::parseMatrix(const JsonCpp::Value& node) {
    if (node.size() != 9) {
        THROW_ERROR("Invalid matrix definition in JSON");
    }
    auto matrix = std::make_unique<GLMatrix>();
    (*matrix)[0][0] = node[0].asFloat();
    (*matrix)[0][1] = node[1].asFloat();
    (*matrix)[0][2] = node[2].asFloat();
    (*matrix)[1][0] = node[3].asFloat();
    (*matrix)[1][1] = node[4].asFloat();
    (*matrix)[1][2] = node[5].asFloat();
    (*matrix)[2][0] = node[6].asFloat();
    (*matrix)[2][1] = node[7].asFloat();
    (*matrix)[2][2] = node[8].asFloat();
    return matrix;
}

SceneNode::MatrixTriple SceneParser::parseMatrices(const JsonCpp::Value& node) {
    std::unique_ptr<GLMatrix> translation = nullptr;
    std::unique_ptr<GLMatrix> rotation = nullptr;
    std::unique_ptr<GLMatrix> scale = nullptr;
    if (node.isMember("translation")) {
        translation = parseMatrix(node["translation"]);
    }
    if (node.isMember("rotation")) {
        rotation = parseMatrix(node["rotation"]);
    }
    if (node.isMember("scale")) {
        scale = parseMatrix(node["scale"]);
    }
    return std::make_tuple(std::move(translation), std::move(rotation), std::move(scale));
}