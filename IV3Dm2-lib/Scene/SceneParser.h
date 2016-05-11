//  Created by David McCann on 5/5/16.
//  Copyright © 2016 Scientific Computing and Imaging Institute. All rights reserved.
//

#pragma once

#include "Communication/ServerAdapter.h"
#include "Scene.h"
#include "SceneMetadata.h"

#include "jsoncpp/json.h"

#include <tuple>

class SceneNode;
class DataProvider;
class ServerAdapter;

class SceneParser {
public:
    SceneParser(const JsonCpp::Value& root, const ServerAdapter& serverAdapter);

    SceneMetadata parseMetadata();
    std::unique_ptr<Scene> parseScene();

private:
    std::unique_ptr<SceneNode> parseNode(const JsonCpp::Value& node);
    std::unique_ptr<SceneNode> parseGeometry(const JsonCpp::Value& node);
    std::unique_ptr<SceneNode> parseGroupNode(const JsonCpp::Value& node);
    std::unique_ptr<DataProvider> parseDataSource(const JsonCpp::Value& node);
    std::unique_ptr<GLMatrix> parseMatrix(const JsonCpp::Value& node);
    SceneNode::MatrixTriple parseMatrices(const JsonCpp::Value& node);
    
private:
    const JsonCpp::Value& m_root;
    const ServerAdapter& m_serverAdapter;
};