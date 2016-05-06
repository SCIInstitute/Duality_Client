//  Created by David McCann on 5/4/16.
//  Copyright © 2016 Scientific Computing and Imaging Institute. All rights reserved.
//

#include "SceneProvider.h"
#include "DatasetProvider.h"

namespace mocca  {
    namespace net {
        class RpcClient;
    }
}

class ServerAdapter : public SceneProvider, DatasetProvider {
public:
    ServerAdapter();
    
    // SceneProvider interface
    std::vector<Scene> listScenes() const override;

    // DatasetProvider interface
    std::unique_ptr<Dataset> downloadDataset(const std::string& path) const override;

private:
    std::unique_ptr<mocca::net::RpcClient> m_rpcClient;
};