//  Created by David McCann on 5/4/16.
//  Copyright © 2016 Scientific Computing and Imaging Institute. All rights reserved.
//

#include "Scene/SceneProvider.h"
#include "Scene/DatasetProvider.h"

namespace mocca  {
    namespace net {
        class RpcClient;
    }
}

class ServerAdapter : public SceneProvider, DatasetProvider {
public:
    ServerAdapter();
    
    // SceneDefinitionProvider interface
    std::vector<SceneMetadata> listMetadata() const override;
    Scene getScene(const std::string& name) const override;

    // DatasetProvider interface
    std::unique_ptr<Dataset> downloadDataset(const std::string& path) const override;

private:
    std::unique_ptr<mocca::net::RpcClient> m_rpcClient;
};