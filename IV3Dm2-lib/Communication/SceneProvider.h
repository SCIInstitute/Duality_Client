//  Created by David McCann on 5/5/16.
//  Copyright © 2016 Scientific Computing and Imaging Institute. All rights reserved.
//

#pragma once

#include "Scene/Scene.h"

#include <vector>

class SceneProvider {
public:
    virtual std::vector<Scene> listScenes() const = 0;
};