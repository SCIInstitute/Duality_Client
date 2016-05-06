//  Created by David McCann on 5/4/16.
//  Copyright © 2016 Scientific Computing and Imaging Institute. All rights reserved.
//

#include "SceneProvider.h"
#include "DatasetProvider.h"
#include "mocca/net/rpc/RpcClient.h"

class ServerAdapter : public SceneProvider, public DatasetProvider {
public:
    ServerAdapter();
    
    // SceneProvider interface
    std::vector<Scene> listScenes() const override;

    // DatasetProvider interface
    std::unique_ptr<Dataset> downloadDataset(const std::string& path) const override;

private:
    std::unique_ptr<mocca::net::RpcClient> m_rpcClient;
};