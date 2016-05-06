//  Created by David McCann on 5/5/16.
//  Copyright © 2016 Scientific Computing and Imaging Institute. All rights reserved.
//

#pragma once

#include "SceneNode.h"

#include <string>

class DownloadNode : public SceneNode {
public:
    DownloadNode(const std::string& path);

    std::string path() const;

private:
    std::string m_path;
};