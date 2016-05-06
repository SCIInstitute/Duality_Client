#include "gtest/gtest.h"

#include "Scene/DownloadNode.h"
#include "Scene/GroupNode.h"
#include "Mocks/DatasetProviderMock.h"

class SceneNodeTest : public ::testing::Test {
protected:
    SceneNodeTest() {}

    virtual ~SceneNodeTest() {}
};

TEST_F(SceneNodeTest, DownloadNode) {
    GroupNode root;
    auto group = std::make_unique<GroupNode>();
    group->addChild(std::make_unique<DownloadNode>("path1"));
    group->addChild(std::make_unique<DownloadNode>("path2"));
    root.addChild(std::move(group));
    root.addChild(std::make_unique<DownloadNode>("path3"));

    DatasetProviderMock mock;
    EXPECT_CALL(mock, downloadDataset("path1")).Times(1);
    EXPECT_CALL(mock, downloadDataset("path2")).Times(1);
    EXPECT_CALL(mock, downloadDataset("path3")).Times(1);

    root.updateDataset(mock);
}