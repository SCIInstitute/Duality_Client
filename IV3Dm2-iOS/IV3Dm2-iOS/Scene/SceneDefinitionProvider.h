//  Created by David McCann on 5/5/16.
//  Copyright © 2016 Scientific Computing and Imaging Institute. All rights reserved.
//

#pragma once

#include "SceneDefinition.h"

#include <vector>

class SceneDefinitionProvider {
public:
    virtual std::vector<SceneDefinition> fetchDefinitions() = 0;
};