#include "gmock/gmock.h"

#include "Scene/GroupNode.h"
#include "Scene/GeometryNode.h"
#include "Mocks/DataProviderMock.h"

using namespace ::testing;

class SceneNodeTest : public Test {
protected:
    SceneNodeTest() {}

    virtual ~SceneNodeTest() {}
};

TEST_F(SceneNodeTest, UpdateDatasets) {
    GroupNode root;
    
    auto group = std::make_unique<GroupNode>();

    auto mock1 = std::make_unique<DataProviderMock>();
    EXPECT_CALL(*mock1, fetch()).Times(1).WillOnce(Return(nullptr));
    group->addChild(std::make_unique<GeometryNode>(std::move(mock1)));

    auto mock2 = std::make_unique<DataProviderMock>();
    EXPECT_CALL(*mock2, fetch()).Times(1).WillOnce(Return(nullptr));
    group->addChild(std::make_unique<GeometryNode>(std::move(mock2)));

    root.addChild(std::move(group));
    
    auto mock3 = std::make_unique<DataProviderMock>();
    EXPECT_CALL(*mock3, fetch()).Times(1).WillOnce(Return(nullptr));
    root.addChild(std::make_unique<GeometryNode>(std::move(mock3)));

    root.updateDataset();
}