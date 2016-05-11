//  Created by David McCann on 5/10/16.
//  Copyright © 2016 Scientific Computing and Imaging Institute. All rights reserved.
//

#include "SceneLoader.h"

#include "Common/Error.h"
#include "Scene/SceneParser.h"
#include "ServerAdapter.h"

#include <algorithm>

SceneLoader::SceneLoader(const ServerAdapter& server)
    : m_server(server) {
    m_root = server.scenes();
    for (auto it = m_root.begin(); it != m_root.end(); ++it) {
        SceneParser parser(*it, m_server);
        m_metadata.push_back(parser.parseMetadata());
    }
}

const std::vector<SceneMetadata>& SceneLoader::listMetadata() const {
    return m_metadata;
}

std::unique_ptr<Scene> SceneLoader::getScene(const std::string& name) const {
    auto it = std::find_if(begin(m_metadata), end(m_metadata), [&name](const SceneMetadata& md) {
        return md.name() == name;
    });
    if (it != end(m_metadata)) {
        auto index = std::distance(begin(m_metadata), it);
        SceneParser parser(m_root[static_cast<int>(index)], m_server);
        return parser.parseScene();
    }
    THROW_ERROR("No scene named " << name);
}
