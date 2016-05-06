//  Created by David McCann on 5/4/16.
//  Copyright © 2016 Scientific Computing and Imaging Institute. All rights reserved.
//

#include "Scene/SceneDefinitionProvider.h"
#include "Scene/DatasetProvider.h"

namespace mocca  {
    namespace net {
        class RpcClient;
    }
}

class ServerAdapter : public SceneDefinitionProvider, DatasetProvider {
public:
    ServerAdapter();
    
    // SceneDefinitionProvider interface
    std::vector<SceneDefinition> fetchDefinitions() const override;
    
    // DatasetProvider interface
    std::unique_ptr<Dataset> downloadDataset(const std::string& path) const override;

private:
    std::unique_ptr<mocca::net::RpcClient> m_rpcClient;
};