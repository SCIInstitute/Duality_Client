#include "gmock/gmock.h"

#include "Mocks/DatasetMock.h"
#include "Mocks/DataProviderMock.h"
#include "Scene/SceneMetadata.h"
#include "Scene/Scene.h"
#include "Scene/SCIRunProvider.h"
#include "Scene/DownloadProvider.h"

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

TEST_F(SceneTest, CreateManipulators) {
    SceneMetadata meta("test", "test");
    Scene scene(meta);

    auto sciRunProvider = std::make_unique<SCIRunProvider>("Test", std::vector<InputParameterFloat>(), std::vector<InputParameterEnum>());
    scene.addNode(std::make_unique<SceneNode>(std::move(sciRunProvider), nullptr));
    auto downloadProvider = std::make_unique<DownloadProvider>(nullptr, "some/path");
    scene.addNode(std::make_unique<SceneNode>(std::move(downloadProvider), nullptr));

    auto manipulators = scene.parameterManipulators();
    ASSERT_EQ(1, manipulators.size());
    ASSERT_TRUE(dynamic_cast<SCIRunProvider*>(manipulators[0]) != nullptr);
}