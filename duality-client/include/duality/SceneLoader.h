//  Created by David McCann on 5/5/16.
//  Copyright © 2016 Scientific Computing and Imaging Institute. All rights reserved.
//

#pragma once

#include "duality/SceneMetadata.h"

#include "mocca/net/Endpoint.h"

#include <vector>
#include <memory>

class SceneLoaderImpl;

class SceneLoader {
public:
    SceneLoader(const mocca::net::Endpoint& endpoint);
    ~SceneLoader();

    std::vector<SceneMetadata> listMetadata() const;
    bool loadScene(const std::string& name);

private:
    std::unique_ptr<SceneLoaderImpl> m_impl;
};