#pragma once

#include "IVDA/Vectors.h"
#include "IVDA/GLMatrix.h"
#include "duality/SceneMetadata.h"
#include "duality/VariableInfo.h"

#include "src/duality/RenderDispatcher3D.h"
#include "src/duality/SceneNode.h"

#include <memory>
#include <string>

class Scene {
public:
    Scene(SceneMetadata metadata);

    SceneMetadata metadata() const;

    void addNode(SceneNode node);
    const std::vector<SceneNode>& nodes() const;
    std::vector<SceneNode>& nodes();

    void dispatch(DatasetDispatcher& dispatcher, View view) const;
    void updateDatasets();

    VariableInfoMap variableInfoMap(View view);
    void setVariable(const std::string& objectName, const std::string& variableName, float value);
    void setVariable(const std::string& objectName, const std::string& variableName, const std::string& value);

private:
    SceneMetadata m_metadata;
    std::vector<SceneNode> m_nodes;
};

namespace duality {
    BoundingBox calculateSceneBoundingBox(const Scene& scene, View view);
}