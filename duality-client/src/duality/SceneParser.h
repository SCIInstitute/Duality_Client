#pragma once

#include "IVDA/Vectors.h"
#include "duality/SceneMetadata.h"
#include "duality/VariableInfo.h"

#include "src/duality/Communication.h"
#include "src/duality/Scene.h"
#include "src/duality/View.h"

#include "jsoncpp/json.h"

class Dataset;
class SceneNode;
class DataProvider;

class SceneParser {
public:
    SceneParser(const JsonCpp::Value& root, std::shared_ptr<LazyRpcClient> rpc);

    std::unique_ptr<Scene> parseScene();
    static SceneMetadata parseMetadata(const JsonCpp::Value& root);

private:
    std::unique_ptr<SceneNode> parseGeometryNode(const JsonCpp::Value& node);
    std::unique_ptr<Dataset> parseGeometryDataset(const JsonCpp::Value& node);

    std::unique_ptr<DataProvider> parseProvider(const JsonCpp::Value& node);
    std::unique_ptr<DataProvider> parseDownload(const JsonCpp::Value& node);
    std::unique_ptr<DataProvider> parsePython(const JsonCpp::Value& node);

    Visibility parseDatasetVisibility(const JsonCpp::Value &node);
    std::unique_ptr<Dataset> parseVolume(const JsonCpp::Value& node);

    IVDA::Mat4f parseMatrix(const JsonCpp::Value& node);
    std::vector<IVDA::Mat4f> parseMatrices(const JsonCpp::Value& node);

    void parseParams(const JsonCpp::Value& node, std::vector<FloatVariableInfo>& floatInfos, std::vector<EnumVariableInfo>& enumInfos);
    FloatVariableInfo parseFloatVariable(const JsonCpp::Value& node, int index);
    EnumVariableInfo parseEnumVariable(const JsonCpp::Value& node, int index);

private:
    JsonCpp::Value m_root;
    std::shared_ptr<LazyRpcClient> m_rpc;
    std::unique_ptr<Scene> m_scene;
};