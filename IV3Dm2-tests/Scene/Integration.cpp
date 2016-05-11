#include "gtest/gtest.h"

#include "Scene/ServerAdapter.h"

#include "mocca/net/ConnectionFactorySelector.h"

class IntegrationTest : public ::testing::Test {
protected:
    IntegrationTest() { mocca::net::ConnectionFactorySelector::addDefaultFactories(); }

    virtual ~IntegrationTest() { mocca::net::ConnectionFactorySelector::removeAll(); }
};

//TEST_F(IntegrationTest, Test1) {
//    ServerAdapter server;
//    auto scenes = server.listScenes();
//    auto& scene = scenes[0];
//    scene.updateDatasets(server);
//}