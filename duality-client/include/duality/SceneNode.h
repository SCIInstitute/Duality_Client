//  Created by David McCann on 5/5/16.
//  Copyright © 2016 Scientific Computing and Imaging Institute. All rights reserved.
//

#pragma once

#include "duality/DataProvider.h"
#include "duality/Dataset.h"
#include "duality/RenderDispatcher3D.h"
#include "duality/BoundingBoxCalculator.h"

#include <memory>
#include <vector>

class SceneNode {
public:
    SceneNode(const std::string& name, std::unique_ptr<DataProvider> provider, std::unique_ptr<Dataset> dataset);

    void updateDataset();
    void dispatch(DatasetDispatcher& dispatcher) const;

    std::string name() const;

    void setVariable(const std::string& variableName, float value);
    void setVariable(const std::string& variableName, const std::string& value);
    
    const DataProvider* dataProvider() const;
    DataProvider* dataProvider();
    const Dataset* dataset() const;
    
private:
    std::string m_name;
    std::unique_ptr<DataProvider> m_provider;
    std::unique_ptr<Dataset> m_dataset;
};