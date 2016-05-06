//  Created by David McCann on 5/5/16.
//  Copyright © 2016 Scientific Computing and Imaging Institute. All rights reserved.
//

#include "GroupNode.h"

void GroupNode::updateDataset(const DatasetProvider& datasetProvider) {
    for (auto& child : m_children) {
        child->updateDataset(datasetProvider);
    }
}

void GroupNode::addChild(std::unique_ptr<SceneNode> child) {
    m_children.push_back(std::move(child));
}

const GroupNode::NodeVec& GroupNode::children() const noexcept {
    return m_children;
}