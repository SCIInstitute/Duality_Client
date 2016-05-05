//  Created by David McCann on 5/4/16.
//  Copyright © 2016 Scientific Computing and Imaging Institute. All rights reserved.
//

#include "SceneDefinitionProvider.h"

namespace mocca  {
    namespace net {
        class RpcClient;
    }
}

class ServerAdapter : public SceneDefinitionProvider {
public:
    ServerAdapter();
    
    std::vector<SceneDefinition> fetchDefinitions() override;
    
private:
    std::unique_ptr<mocca::net::RpcClient> m_rpcClient;
};