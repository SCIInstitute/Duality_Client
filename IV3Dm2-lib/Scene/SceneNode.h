//  Created by David McCann on 5/5/16.
//  Copyright © 2016 Scientific Computing and Imaging Institute. All rights reserved.
//

#pragma once

#include "Scene/DataProvider.h"
#include "IVDA/Vectors.h"

#include <memory>
#include <tuple>
#include <vector>

class AbstractDispatcher;

class SceneNode {
public:
    SceneNode(std::unique_ptr<DataProvider> provider, std::vector<IVDA::Mat4f> transforms = {});
    virtual ~SceneNode() {}

    virtual void applyTransform(const IVDA::Mat4f& matrix) = 0;
    virtual void accept(AbstractDispatcher& dispatcher) = 0;
    virtual void readDataset(std::shared_ptr<std::vector<uint8_t>> data) = 0;
    virtual void updateDataset();

    const DataProvider& dataProvider() const;
    const std::vector<IVDA::Mat4f>& transforms() const;

private:
    std::unique_ptr<DataProvider> m_provider;
    std::vector<IVDA::Mat4f> m_transforms;
};