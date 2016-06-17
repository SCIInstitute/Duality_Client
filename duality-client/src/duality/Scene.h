#pragma once

#include "IVDA/GLMatrix.h"
#include "IVDA/Vectors.h"
#include "duality/InputVariable.h"
#include "duality/SceneMetadata.h"

#include "src/duality/RenderDispatcher3D.h"
#include "src/duality/SceneNode.h"

#include <memory>
#include <string>

class Scene {
public:
    Scene(SceneMetadata metadata, std::vector<std::unique_ptr<SceneNode>> nodes,
          std::map<std::string, std::shared_ptr<Variables>> variables);
    
    SceneMetadata metadata() const;

    const std::vector<std::unique_ptr<SceneNode>>& nodes() const;

    void dispatch(NodeDispatcher& dispatcher, View view) const;
    void updateDatasets();

    VariableMap variableMap(View view);
    void setVariable(const std::string& objectName, const std::string& variableName, float value);
    void setVariable(const std::string& objectName, const std::string& variableName, const std::string& value);

private:
    SceneMetadata m_metadata;
    std::vector<std::unique_ptr<SceneNode>> m_nodes;
    std::map<std::string, std::shared_ptr<Variables>> m_variables;
};

namespace duality {
BoundingBox calculateSceneBoundingBox(const Scene& scene, View view);
}