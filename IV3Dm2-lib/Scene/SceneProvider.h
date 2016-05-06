//  Created by David McCann on 5/5/16.
//  Copyright © 2016 Scientific Computing and Imaging Institute. All rights reserved.
//

#pragma once

#include "Scene.h"

#include <vector>

class SceneProvider {
public:
    virtual std::vector<SceneMetadata> listMetadata() const = 0;
    virtual Scene getScene(const std::string& name) const = 0;
};