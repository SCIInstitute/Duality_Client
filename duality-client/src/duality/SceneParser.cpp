//  Created by David McCann on 5/5/16.
//  Copyright © 2016 Scientific Computing and Imaging Institute. All rights reserved.
//

#include "duality/SceneParser.h"

#include "duality/Error.h"
#include "duality/DownloadProvider.h"
#include "duality/GeometryDataset.h"
#include "duality/PythonProvider.h"

using namespace IVDA;

SceneParser::SceneParser(const JsonCpp::Value& root, std::shared_ptr<LazyRpcClient> rpc)
    : m_root(root)
    , m_rpc(rpc) {}

SceneMetadata SceneParser::parseMetadata(const JsonCpp::Value& root) {
    std::string name = root["metadata"]["name"].asString();
    std::string description = root["metadata"]["description"].asString();
    return SceneMetadata(std::move(name), std::move(description));
}

std::unique_ptr<Scene> SceneParser::parseScene() {
    m_scene = std::make_unique<Scene>(parseMetadata(m_root));
    auto sceneJson = m_root["scene"];
    for (auto it = sceneJson.begin(); it != sceneJson.end(); ++it) {
        m_scene->addNode(parseNode(*it));
    }
    return std::move(m_scene);
}

SceneNode SceneParser::parseNode(const JsonCpp::Value& node) {
    std::string name = node["name"].asString();
    auto provider = parseDataSource(node["source"]);
    auto dataset = parseDataset(node["dataset"]);
    return SceneNode(name, std::move(provider), std::move(dataset));
}

std::unique_ptr<Dataset> SceneParser::parseDataset(const JsonCpp::Value& node) {
    std::string type = node["type"].asString();
    if (type == "geometry") {
        return parseGeometry(node);
    }
    throw Error("Invalid node type: " + type, __FILE__, __LINE__);
}

std::unique_ptr<Dataset> SceneParser::parseGeometry(const JsonCpp::Value& node) {
    std::vector<Mat4f> transforms;
    if (node.isMember("transforms")) {
        transforms = parseMatrices(node["transforms"]);
    }
    return std::make_unique<GeometryDataset>(std::move(transforms));
}

std::unique_ptr<DataProvider> SceneParser::parseDataSource(const JsonCpp::Value& node) {
    std::string type = node["type"].asString();
    if (type == "download") {
        return parseDownload(node);
    } else if (type == "python") {
        return parsePython(node);
    }
    throw Error("Invalid data source type: " + type, __FILE__, __LINE__);
}

std::unique_ptr<DataProvider> SceneParser::parseDownload(const JsonCpp::Value& node) {
    return std::make_unique<DownloadProvider>(m_scene->metadata().name(), node["filename"].asString(), m_rpc);
}

Mat4f SceneParser::parseMatrix(const JsonCpp::Value& node) {
    if (node.size() != 16) {
        throw Error("Invalid matrix definition in JSON", __FILE__, __LINE__);
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

std::unique_ptr<DataProvider> SceneParser::parsePython(const JsonCpp::Value& node) {
    std::vector<InputVariableFloat> floatVariables;
    std::vector<InputVariableEnum> enumVariables;
    std::string fileName = node["filename"].asString();
    parseParams(node["variables"], floatVariables, enumVariables);
    return std::make_unique<PythonProvider>(m_scene->metadata().name(), fileName, std::move(floatVariables), std::move(enumVariables),
                                            m_rpc);
}

void SceneParser::parseParams(const JsonCpp::Value& node, std::vector<InputVariableFloat>& floatParams,
                              std::vector<InputVariableEnum>& enumParams) {
    int index = 0;
    for (auto it = node.begin(); it != node.end(); ++it, ++index) {
        auto paramNode = *it;
        if (paramNode["type"] == "float") {
            floatParams.push_back(parseFloatVariable(paramNode, index));
        } else if (paramNode["type"] == "enum") {
            enumParams.push_back(parseEnumVariable(paramNode, index));
        }
    }
}

InputVariableFloat SceneParser::parseFloatVariable(const JsonCpp::Value& node, int index) {
    std::string name = node["name"].asString();
    float lowerBound = node["lowerBound"].asFloat();
    float upperBound = node["upperBound"].asFloat();
    float stepSize = node["stepSize"].asFloat();
    float defaultValue = node["defaultValue"].asFloat();
    InputVariableFloat::Info info{std::move(name), index, lowerBound, upperBound, stepSize, defaultValue};
    return InputVariableFloat(info);
}

InputVariableEnum SceneParser::parseEnumVariable(const JsonCpp::Value& node, int index) {
    std::string name = node["name"].asString();
    auto valuesNode = node["values"];
    std::vector<std::string> values;
    for (auto valIt = valuesNode.begin(); valIt != valuesNode.end(); ++valIt) {
        values.push_back(valIt->asString());
    }
    std::string defaultValue = node["defaultValue"].asString();
    InputVariableEnum::Info info{std::move(name), index, std::move(values), std::move(defaultValue)};
    return InputVariableEnum(info);
}