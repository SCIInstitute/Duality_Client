//  Created by David McCann on 5/5/16.
//  Copyright © 2016 Scientific Computing and Imaging Institute. All rights reserved.
//

#pragma once

class DatasetProvider;

class SceneNode {
public:
    virtual ~SceneNode() {}

    virtual void updateDataset(const DatasetProvider& datasetProvider) = 0;
    //virtual void render(Renderer& renderer) const = 0;
};