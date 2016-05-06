//  Created by David McCann on 5/5/16.
//  Copyright © 2016 Scientific Computing and Imaging Institute. All rights reserved.
//

#pragma once

#include "SceneMetadata.h"
#include "SceneNode.h"

#include "jsoncpp/json.h"

#include <string>
#include <memory>

class Scene {
public:
    Scene(SceneMetadata metadata, std::unique_ptr<SceneNode> sceneRoot);
    Scene(const Scene& other);

    static Scene fromJson(const JsonCpp::Value& root);
    
    SceneMetadata metadata() const noexcept;
    const SceneNode& rootNode() const noexcept;

    void updateDatasets(const DatasetProvider& provider);

private:
    SceneMetadata m_metadata;
    std::unique_ptr<SceneNode> m_sceneRoot;
};