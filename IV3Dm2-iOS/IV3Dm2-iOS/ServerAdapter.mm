//
//  ServerAdapter.m
//  IV3Dm2-iOS
//
//  Created by David McCann on 5/4/16.
//  Copyright © 2016 Scientific Computing and Imaging Institute. All rights reserved.
//

#include "ServerAdapter.h"

#include "mocca/net/rpc/RpcClient.h"

ServerAdapter::ServerAdapter() {
    mocca::net::Endpoint ep("tcp.prefixed", "192.168.1.222", "10123");
    m_rpcClient = new mocca::net::RpcClient(ep);
}
