//  Created by David McCann on 5/4/16.
//  Copyright © 2016 Scientific Computing and Imaging Institute. All rights reserved.
//

#include "SceneProvider.h"

#include "mocca/net/rpc/RpcClient.h"

class ServerAdapter : public SceneProvider {
public:
    ServerAdapter();
    
    // SceneProvider interface
    std::vector<Scene> listScenes() const override;

    std::shared_ptr<std::vector<uint8_t>> download(const std::string& path) const;

private:
    std::unique_ptr<mocca::net::RpcClient> m_rpcClient;
};