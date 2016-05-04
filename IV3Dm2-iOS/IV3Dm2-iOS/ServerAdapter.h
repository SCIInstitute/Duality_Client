//
//  ServerAdapter.h
//  IV3Dm2-iOS
//
//  Created by David McCann on 5/4/16.
//  Copyright © 2016 Scientific Computing and Imaging Institute. All rights reserved.
//

namespace mocca  {
    namespace net {
        class RpcClient;
    }
}

class ServerAdapter {
public:
    ServerAdapter();
    
private:
    mocca::net::RpcClient* m_rpcClient;
};