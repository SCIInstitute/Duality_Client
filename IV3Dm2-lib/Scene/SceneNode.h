//  Created by David McCann on 5/5/16.
//  Copyright © 2016 Scientific Computing and Imaging Institute. All rights reserved.
//

#pragma once

#include "Scene/DataProvider.h"
#include "Scene/Dataset.h"
#include "Scene/UpdateDataDispatcher.h"
#include "Scene/RenderDispatcher.h"
#include "Scene/BoundingBoxCalculator.h"
#include "Scene/ParameterManipulator.h"

#include <memory>
#include <vector>

class SceneNode {
public:
    SceneNode(std::unique_ptr<DataProvider> provider, std::unique_ptr<Dataset> dataset);

    void updateDatasets(UpdateDataDispatcher& dispatcher);
    void render(RenderDispatcher& dispatcher) const;
    void calculateBoundingBox(BoundingBoxCalculator& dispatcher) const;
    void makeManipultor(ParameterManipulatorCollector& dispatcher) const;

    const DataProvider* dataProvider() const;
    const Dataset* dataset() const;

private:
    void loadDataset(std::shared_ptr<std::vector<uint8_t>> data);
    
private:
    std::unique_ptr<DataProvider> m_provider;
    std::unique_ptr<Dataset> m_dataset;
};