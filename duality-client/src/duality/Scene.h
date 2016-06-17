#pragma once

#include "IVDA/Vectors.h"
#include "IVDA/GLMatrix.h"
#include "duality/SceneMetadata.h"
#include "duality/InputVariable.h"

#include "src/duality/RenderDispatcher3D.h"
#include "src/duality/SceneNode.h"

#include <memory>
#include <string>

class Scene {
public:
    Scene(SceneMetadata metadata);

    SceneMetadata metadata() const;

    void addNode(std::unique_ptr<SceneNode> node);
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