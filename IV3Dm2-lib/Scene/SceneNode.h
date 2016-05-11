//  Created by David McCann on 5/5/16.
//  Copyright © 2016 Scientific Computing and Imaging Institute. All rights reserved.
//

#pragma once

#include "Scene/DataProvider.h"
#include "Scene/GLMatrix.h"

#include <memory>
#include <tuple>
#include <vector>

class AbstractDispatcher;

class SceneNode {
public:
    // translation, rotation, scaling
    using MatrixTriple = std::tuple<std::unique_ptr<GLMatrix>, std::unique_ptr<GLMatrix>, std::unique_ptr<GLMatrix>>;

    SceneNode(std::unique_ptr<DataProvider> provider, MatrixTriple transforms = MatrixTriple{})
        : m_provider(std::move(provider))
        , m_transforms(std::move(transforms)) {}
    virtual ~SceneNode() {}

    virtual void accept(AbstractDispatcher& dispatcher) = 0;
    virtual void readDataset(std::shared_ptr<std::vector<uint8_t>> data) = 0;

    virtual void updateDataset() {
        if (m_provider != nullptr) {
            auto data = m_provider->fetch();
            if (data != nullptr) {
                readDataset(data);
            }
        }
    }

    const DataProvider& dataProvider() const {
        return *m_provider;
    }

    const MatrixTriple& transforms() const {
        return m_transforms;
    }

private:
    std::unique_ptr<DataProvider> m_provider;
    MatrixTriple m_transforms;
};