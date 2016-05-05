//  Created by David McCann on 5/5/16.
//  Copyright © 2016 Scientific Computing and Imaging Institute. All rights reserved.
//

#pragma once

#include <string>

class SceneDefinition {
public:
    SceneDefinition(const std::string& name);
    
    std::string name() const;
    
private:
    std::string m_name;
};