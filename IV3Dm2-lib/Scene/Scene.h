//  Created by David McCann on 5/5/16.
//  Copyright © 2016 Scientific Computing and Imaging Institute. All rights reserved.
//

#pragma once

#include "Scene/GLMatrix.h"
#include "Scene/SceneMetadata.h"
#include "Scene/SceneNode.h"
#include "IVDA/Vectors.h"

#include <memory>
#include <string>

class AbstractDispatcher;
class RenderDispatcher;

class Scene {
public:
    Scene(SceneMetadata metadata);

    SceneMetadata metadata() const;

    void addNode(std::unique_ptr<SceneNode> node);
    const std::vector<std::unique_ptr<SceneNode>>& nodes() const;

    void updateDatasets();
    void dispatch(AbstractDispatcher& dispatcher) const;

    void addTranslation(const IVDA::Vec2f& translation);
    GLMatrix modelViewMatrix() const;

private:
    GLMatrix defaultModelView() const;

private:
    SceneMetadata m_metadata;
    std::vector<std::unique_ptr<SceneNode>> m_nodes;
    GLMatrix m_defaultModelView;
    IVDA::Vec3f m_translation;
};