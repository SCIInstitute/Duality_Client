#include "gmock/gmock.h"

#include "Mocks/DatasetMock.h"
#include "Mocks/DataProviderMock.h"
#include "Scene/SceneMetadata.h"
#include "Scene/Scene.h"

using namespace ::testing;

class SceneTest : public Test {
protected:
    SceneTest() {}

    virtual ~SceneTest() {}
};

TEST_F(SceneTest, UpdateDatasets) {
    SceneMetadata meta("test", "test");
    Scene scene(meta);

    auto providerMock1 = std::make_unique<DataProviderMock>();
    EXPECT_CALL(*providerMock1, accept(_)).Times(1);
    auto datasetMock1 = std::make_unique<DatasetMock>();
    EXPECT_CALL(*datasetMock1, accept(_)).Times(1);
    EXPECT_CALL(*datasetMock1, read(_)).Times(1);
    scene.addNode(std::make_unique<SceneNode>(std::move(providerMock1), std::move(datasetMock1)));

    auto providerMock2 = std::make_unique<DataProviderMock>();
    EXPECT_CALL(*providerMock2, accept(_)).Times(1);
    auto datasetMock2 = std::make_unique<DatasetMock>();
    EXPECT_CALL(*datasetMock2, accept(_)).Times(1);
    EXPECT_CALL(*datasetMock2, read(_)).Times(1);
    scene.addNode(std::make_unique<SceneNode>(std::move(providerMock2), std::move(datasetMock2)));

    scene.updateDatasets();
}

TEST_F(SceneTest, DefaultModelView) {
    // TODO: write test
}