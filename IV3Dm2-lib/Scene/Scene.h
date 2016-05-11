//  Created by David McCann on 5/5/16.
//  Copyright © 2016 Scientific Computing and Imaging Institute. All rights reserved.
//

#pragma once

#include "Scene/SceneMetadata.h"
#include "Scene/SceneNode.h"

#include <string>
#include <memory>

class RenderDispatcher;

class Scene {
public:
    Scene(SceneMetadata metadata);

    SceneMetadata metadata() const noexcept;

    void addNode(std::unique_ptr<SceneNode> node);
    const std::vector<std::unique_ptr<SceneNode>>& nodes() const noexcept;

    void updateDatasets();
    void render(RenderDispatcher& dispatcher) const;

private:
    SceneMetadata m_metadata;
    std::vector<std::unique_ptr<SceneNode>> m_nodes;
};