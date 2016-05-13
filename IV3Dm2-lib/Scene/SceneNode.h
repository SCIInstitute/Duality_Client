//  Created by David McCann on 5/5/16.
//  Copyright © 2016 Scientific Computing and Imaging Institute. All rights reserved.
//

#pragma once

#include "Scene/DataProvider.h"
#include "Scene/Dataset.h"

#include <memory>
#include <vector>

class DatasetDispatcher;

class SceneNode {
public:
    SceneNode(std::unique_ptr<DataProvider> provider, std::unique_ptr<Dataset> dataset);

    void dispatch(DatasetDispatcher& dispatcher);
    void dispatch(DataProviderDispatcher& dispatcher);
    
    void loadDataset(std::shared_ptr<std::vector<uint8_t>> data);

    const DataProvider* dataProvider() const;
    const Dataset* dataset() const;

private:
    std::unique_ptr<DataProvider> m_provider;
    std::unique_ptr<Dataset> m_dataset;
};