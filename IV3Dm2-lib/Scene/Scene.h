//  Created by David McCann on 5/5/16.
//  Copyright © 2016 Scientific Computing and Imaging Institute. All rights reserved.
//

#pragma once

#include "IVDA/Vectors.h"
#include "Scene/GLMatrix.h"
#include "Scene/RenderDispatcher.h"
#include "Scene/SceneMetadata.h"
#include "Scene/SceneNode.h"

#include <memory>
#include <string>

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

    void addTranslation(const IVDA::Vec2f& translation);
    void addRotation(const IVDA::Mat4f& rotation);
    GLMatrix modelViewMatrix() const;

private:
    GLMatrix defaultModelView() const;

private:
    SceneMetadata m_metadata;
    std::vector<SceneNode> m_nodes;
    GLMatrix m_defaultModelView;
    IVDA::Vec3f m_translation;
    IVDA::Mat4f m_rotation;
};