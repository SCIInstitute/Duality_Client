//  Created by David McCann on 5/5/16.
//  Copyright © 2016 Scientific Computing and Imaging Institute. All rights reserved.
//

#pragma once

#include "Communication/DataProvider.h"

#include <memory>

class AbstractDispatcher;

class SceneNode {
public:
    SceneNode(std::unique_ptr<DataProvider> provider)
        : m_provider(std::move(provider)) {}
    virtual ~SceneNode() {}

    virtual void accept(AbstractDispatcher& dispatcher) = 0;
    virtual void readDataset(std::shared_ptr<std::vector<uint8_t>> data) = 0;

    void updateDataset() {
        auto data = m_provider->fetch();
        readDataset(data);
    }

private:
    std::unique_ptr<DataProvider> m_provider;
};