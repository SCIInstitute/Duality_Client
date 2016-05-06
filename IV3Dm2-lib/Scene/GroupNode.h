//  Created by David McCann on 5/5/16.
//  Copyright © 2016 Scientific Computing and Imaging Institute. All rights reserved.
//

#pragma once

#include "SceneNode.h"

#include <vector>
#include <memory>

class GroupNode : public SceneNode {
public:
    using NodeVec = std::vector<std::unique_ptr<SceneNode>>;

    void updateDatasets(const DatasetProvider& datasetProvider) override;

    std::unique_ptr<SceneNode> clone() const override;

    void addChild(std::unique_ptr<SceneNode> child);
    const NodeVec& children() const noexcept;

private:
    NodeVec m_children;
};