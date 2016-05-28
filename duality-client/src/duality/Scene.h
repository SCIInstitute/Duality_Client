//  Created by David McCann on 5/5/16.
//  Copyright © 2016 Scientific Computing and Imaging Institute. All rights reserved.
//

#pragma once

#include "IVDA/Vectors.h"
#include "IVDA/GLMatrix.h"
#include "duality/SceneMetadata.h"
#include "src/duality/RenderDispatcher3D.h"
#include "src/duality/SceneNode.h"

#include <memory>
#include <string>
#include <map>

class Scene {
public:
    Scene(SceneMetadata metadata);

    SceneMetadata metadata() const;

    void addNode(SceneNode node);
    const std::vector<SceneNode>& nodes() const;
    std::vector<SceneNode>& nodes();

    void dispatch(DatasetDispatcher& dispatcher) const;
    void updateDatasets();

    struct VariableInfos {
        std::vector<InputVariableFloat::Info> floatInfos;
        std::vector<InputVariableEnum::Info> enumInfos;
    };
    using VariableMap = std::map<std::string, VariableInfos>;
    VariableMap variableMap();
    void setVariable(const std::string& objectName, const std::string& variableName, float value);
    void setVariable(const std::string& objectName, const std::string& variableName, const std::string& value);

private:
    SceneMetadata m_metadata;
    std::vector<SceneNode> m_nodes;
};