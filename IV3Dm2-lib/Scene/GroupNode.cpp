//  Created by David McCann on 5/5/16.
//  Copyright © 2016 Scientific Computing and Imaging Institute. All rights reserved.
//

#include "GroupNode.h"

#include "AbstractDispatcher.h"

GroupNode::GroupNode(std::unique_ptr<DataProvider> provider, SceneNode::MatrixTriple transforms)
    : SceneNode(std::move(provider), std::move(transforms)) {}

void GroupNode::accept(AbstractDispatcher& dispatcher) {
    dispatcher.dispatch(*this);
}

void GroupNode::updateDataset() {
    for (auto& child : m_children) {
        child->updateDataset();
    }
}

void GroupNode::readDataset(std::shared_ptr<std::vector<uint8_t>> data) {}

void GroupNode::addChild(std::unique_ptr<SceneNode> child) {
    m_children.push_back(std::move(child));
}

const GroupNode::NodeVec& GroupNode::children() const noexcept {
    return m_children;
}