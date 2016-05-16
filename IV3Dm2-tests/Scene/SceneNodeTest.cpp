#include "gmock/gmock.h"

#include "Mocks/DataProviderMock.h"
#include "Mocks/DatasetMock.h"
#include "Scene/DownloadProvider.h"
#include "Scene/SCIRunProvider.h"
#include "Scene/Scene.h"
#include "Scene/SceneMetadata.h"

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
    scene.addNode(std::make_unique<SceneNode>(std::move(providerMock1), std::move(datasetMock1)));

    auto providerMock2 = std::make_unique<DataProviderMock>();
    EXPECT_CALL(*providerMock2, accept(_)).Times(1);
    auto datasetMock2 = std::make_unique<DatasetMock>();
    EXPECT_CALL(*datasetMock2, accept(_)).Times(1);
    scene.addNode(std::make_unique<SceneNode>(std::move(providerMock2), std::move(datasetMock2)));

    scene.updateDatasets();
}

TEST_F(SceneTest, DefaultModelView) {
    // TODO: write test
}

TEST_F(SceneTest, CreateFloatManipulators) {
    SceneMetadata meta("test", "test");
    Scene scene(meta);

    InputParameterFloat floatParam1("floatParam1", 0, 0.f, 10.f, 1.f, 5.f);
    InputParameterFloat floatParam2("floatParam2", 0, 0.f, 10.f, 1.f, 5.f);

    auto sciRunProvider = std::make_unique<SCIRunProvider>(nullptr, "Test", std::vector<InputParameterFloat>{floatParam1, floatParam2},
                                                           std::vector<InputParameterEnum>());
    scene.addNode(std::make_unique<SceneNode>(std::move(sciRunProvider), nullptr));
    auto downloadProvider = std::make_unique<DownloadProvider>(nullptr, "some/path");
    scene.addNode(std::make_unique<SceneNode>(std::move(downloadProvider), nullptr));

    auto manipulators = scene.manipulators().first;
    ASSERT_EQ(2, manipulators.size());
    ASSERT_EQ("floatParam1", manipulators[0].parameter().name);
    ASSERT_EQ("floatParam2", manipulators[1].parameter().name);
}

TEST_F(SceneTest, CreateEnumManipulators) {
    SceneMetadata meta("test", "test");
    Scene scene(meta);

    InputParameterEnum enumParam1("enumParam1", 0, {"val1", "val2"}, "val2");
    InputParameterEnum enumParam2("enumParam2", 1, {"val1", "val2"}, "val2");

    auto sciRunProvider = std::make_unique<SCIRunProvider>(nullptr, "Test", std::vector<InputParameterFloat>(),
                                                           std::vector<InputParameterEnum>{enumParam1, enumParam2});
    scene.addNode(std::make_unique<SceneNode>(std::move(sciRunProvider), nullptr));
    auto downloadProvider = std::make_unique<DownloadProvider>(nullptr, "some/path");
    scene.addNode(std::make_unique<SceneNode>(std::move(downloadProvider), nullptr));

    auto manipulators = scene.manipulators().second;
    ASSERT_EQ(2, manipulators.size());
    ASSERT_EQ("enumParam1", manipulators[0].parameter().name);
    ASSERT_EQ("enumParam2", manipulators[1].parameter().name);
}