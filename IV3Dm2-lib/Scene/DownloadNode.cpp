//  Created by David McCann on 5/5/16.
//  Copyright © 2016 Scientific Computing and Imaging Institute. All rights reserved.
//

#include "DownloadNode.h"

DownloadNode::DownloadNode(const std::string& path)
    : m_path(path) {}

std::string DownloadNode::path() const {
    return m_path;
}
