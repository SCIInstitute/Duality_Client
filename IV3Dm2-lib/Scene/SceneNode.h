//  Created by David McCann on 5/5/16.
//  Copyright © 2016 Scientific Computing and Imaging Institute. All rights reserved.
//

#pragma once

#include <memory>

class DatasetProvider;

class SceneNode {
public:
    virtual ~SceneNode() {}

    virtual void updateDatasets(const DatasetProvider& datasetProvider) = 0;
    //virtual void render(Renderer& renderer) const = 0;

    virtual std::unique_ptr<SceneNode> clone() const = 0;
};