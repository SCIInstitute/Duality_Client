//  Created by David McCann on 5/5/16.
//  Copyright © 2016 Scientific Computing and Imaging Institute. All rights reserved.
//

#pragma once

#include "Scene/InputParameter.h"
#include "Scene/Scene.h"
#include "Scene/SceneMetadata.h"
#include "Scene/ServerAdapter.h"

#include "IVDA/Vectors.h"

#include "jsoncpp/json.h"

class Dataset;
class SceneNode;
class DataProvider;
class ServerAdapter;

class SceneParser {
public:
    SceneParser(const JsonCpp::Value& root, const ServerAdapter* serverAdapter);

    SceneMetadata parseMetadata();
    std::unique_ptr<Scene> parseScene();

private:
    std::unique_ptr<SceneNode> parseNode(const JsonCpp::Value& node);
    
    std::unique_ptr<Dataset> parseDataset(const JsonCpp::Value& node);
    std::unique_ptr<Dataset> parseGeometry(const JsonCpp::Value& node);
    
    IVDA::Mat4f parseMatrix(const JsonCpp::Value& node);
    std::vector<IVDA::Mat4f> parseMatrices(const JsonCpp::Value& node);
    
    std::unique_ptr<DataProvider> parseDataSource(const JsonCpp::Value& node);
    std::unique_ptr<DataProvider> parseDownload(const JsonCpp::Value& node);
    std::unique_ptr<DataProvider> parseSCIRun(const JsonCpp::Value& node);
    std::vector<InputParameterFloat> parseFloatParams(const JsonCpp::Value& node);
    std::vector<InputParameterEnum> parseEnumParams(const JsonCpp::Value& node);

private:
    JsonCpp::Value m_root;
    const ServerAdapter* m_serverAdapter;
};