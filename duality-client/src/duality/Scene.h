#pragma once

#include "IVDA/GLMatrix.h"
#include "IVDA/Vectors.h"
#include "duality/InputVariable.h"
#include "duality/SceneMetadata.h"

#include "src/duality/RenderDispatcher2D.h"
#include "src/duality/RenderDispatcher3D.h"
#include "src/duality/SceneNode.h"

#include "mocca/base/Nullable.h"

#include <memory>
#include <string>
#include <functional>

class Scene {
public:
    Scene(SceneMetadata metadata, std::vector<std::unique_ptr<SceneNode>> nodes,
          std::map<std::string, std::shared_ptr<Variables>> variables, const std::string& webViewURL);

    SceneMetadata metadata() const;
    std::string webViewURL() const;

    const std::vector<std::unique_ptr<SceneNode>>& nodes() const;
    std::vector<const GeometryNode*> geometryNodes() const;
    std::vector<const VolumeNode*> volumeNodes() const;

    void updateDatasets();
    void initializeDatasets();
    void setUpdateDatasetCallback(std::function<void(int,int,const std::string&)> callback);

    BoundingBox boundingBox(View view) const;
    
    VariableMap variableMap(View view);
    void setVariable(const std::string& objectName, const std::string& variableName, float value);
    void setVariable(const std::string& objectName, const std::string& variableName, const std::string& value);

private:
    SceneMetadata m_metadata;
    std::vector<std::unique_ptr<SceneNode>> m_nodes;
    std::map<std::string, std::shared_ptr<Variables>> m_variables;
    std::string m_webViewURL;
    mocca::Nullable<std::function<void(int,int,const std::string&)>> m_updateDatasetCallback;
};