#include "src/duality/SceneParser.h"

#include "duality/Error.h"
#include "src/duality/DownloadProvider.h"
#include "src/duality/GeometryDataset.h"
#include "src/duality/GeometryNode.h"
#include "src/duality/PythonProvider.h"
#include "src/duality/VolumeDataset.h"
#include "src/duality/VolumeNode.h"

#include "mocca/log/LogManager.h"

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
        const auto& sceneNode = *it;
        auto type = sceneNode["type"].asString();
        if (type == "geometry") {
            m_scene->addNode(parseGeometryNode(sceneNode));
        } else if (type == "volume") {
            m_scene->addNode(parseVolumeNode(sceneNode));
        } else {
            throw Error("Invalid node type: " + type, __FILE__, __LINE__);
        }
    }
    return std::move(m_scene);
}

std::unique_ptr<SceneNode> SceneParser::parseGeometryNode(const JsonCpp::Value& node) {
    std::string name = node["name"].asString();
    Visibility visibility = parseVisibility(node);
    auto dataset = parseGeometryDataset(node["dataset"]);
    return std::make_unique<GeometryNode>(name, visibility, std::move(dataset));
}

std::unique_ptr<GeometryDataset> SceneParser::parseGeometryDataset(const JsonCpp::Value& node) {
    auto provider = parseProvider(node["source"]);
    std::vector<Mat4f> transforms;
    if (node.isMember("transforms")) {
        transforms = parseMatrices(node["transforms"]);
    }
    return std::make_unique<GeometryDataset>(std::move(provider), std::move(transforms));
}

std::unique_ptr<SceneNode> SceneParser::parseVolumeNode(const JsonCpp::Value& node) {
    std::string name = node["name"].asString();
    Visibility visibility = parseVisibility(node);
    auto dataset = parseVolumeDataset(node["dataset"]);
    return std::make_unique<VolumeNode>(name, visibility, std::move(dataset));
}

std::unique_ptr<VolumeDataset> SceneParser::parseVolumeDataset(const JsonCpp::Value& node) {
    auto provider = parseProvider(node["source"]);
    return std::make_unique<VolumeDataset>(std::move(provider));
}

std::unique_ptr<DataProvider> SceneParser::parseProvider(const JsonCpp::Value& node) {
    std::string type = node["type"].asString();
    if (type == "download") {
        return parseDownload(node);
    } else if (type == "python") {
        return parsePython(node);
    }
    throw Error("Invalid data source type: " + type, __FILE__, __LINE__);
}

Visibility SceneParser::parseVisibility(const JsonCpp::Value& node) {
    Visibility visibility = Visibility::VisibleBoth;
    if (node.isMember("view2d") && node["view2d"] == false) {
        visibility = Visibility::Visible3D;
    }
    if (node.isMember("view3d") && node["view3d"] == false) {
        visibility = Visibility::Visible2D;
    }
    if (node.isMember("view2d") && node["view2d"] == false && node.isMember("view3d") && node["view3d"] == false) {
        visibility = Visibility::VisibleNone;
        LWARNING("Node is invisible in 2d-view and 3d-view");
    }
    return visibility;
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
    std::vector<FloatVariableInfo> floatInfos;
    std::vector<EnumVariableInfo> enumInfos;
    std::string fileName = node["filename"].asString();
    parseParams(node["variables"], floatInfos, enumInfos);
    return std::make_unique<PythonProvider>(m_scene->metadata().name(), fileName, std::move(floatInfos), std::move(enumInfos), m_rpc);
}

void SceneParser::parseParams(const JsonCpp::Value& node, std::vector<FloatVariableInfo>& floatInfos,
                              std::vector<EnumVariableInfo>& enumInfos) {
    int index = 0;
    for (auto it = node.begin(); it != node.end(); ++it, ++index) {
        auto paramNode = *it;
        if (paramNode["type"] == "float") {
            floatInfos.push_back(parseFloatVariable(paramNode, index));
        } else if (paramNode["type"] == "enum") {
            enumInfos.push_back(parseEnumVariable(paramNode, index));
        }
    }
}

FloatVariableInfo SceneParser::parseFloatVariable(const JsonCpp::Value& node, int index) {
    std::string name = node["name"].asString();
    float lowerBound = node["lowerBound"].asFloat();
    float upperBound = node["upperBound"].asFloat();
    float stepSize = node["stepSize"].asFloat();
    float defaultValue = node["defaultValue"].asFloat();
    FloatVariableInfo info{std::move(name), index, lowerBound, upperBound, stepSize, defaultValue};
    return info;
}

EnumVariableInfo SceneParser::parseEnumVariable(const JsonCpp::Value& node, int index) {
    std::string name = node["name"].asString();
    auto valuesNode = node["values"];
    std::vector<std::string> values;
    for (auto valIt = valuesNode.begin(); valIt != valuesNode.end(); ++valIt) {
        values.push_back(valIt->asString());
    }
    std::string defaultValue = node["defaultValue"].asString();
    EnumVariableInfo info{std::move(name), index, std::move(values), std::move(defaultValue)};
    return info;
}
