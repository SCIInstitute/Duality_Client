#include "gtest/gtest.h"

#include "DataProviderMock.h"
#include "DatasetMock.h"
#include "duality/SceneMetadata.h"
#include "src/duality/DownloadProvider.h"
#include "src/duality/PythonProvider.h"
#include "src/duality/Scene.h"

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
    EXPECT_CALL(*providerMock1, fetch()).Times(1).WillOnce(Return(nullptr));
    auto datasetMock1 = std::make_unique<DatasetMock>();
    EXPECT_CALL(*datasetMock1, accept(_)).Times(1);
    scene.addNode(SceneNode("node", std::move(providerMock1), std::move(datasetMock1)));

    auto providerMock2 = std::make_unique<DataProviderMock>();
    EXPECT_CALL(*providerMock2, fetch()).Times(1).WillOnce(Return(nullptr));
    auto datasetMock2 = std::make_unique<DatasetMock>();
    EXPECT_CALL(*datasetMock2, accept(_)).Times(1);
    scene.addNode(SceneNode("node", std::move(providerMock2), std::move(datasetMock2)));

    scene.updateDatasets();
}

TEST_F(SceneTest, FloatVariables) {
    FloatVariableInfo floatInfo1{"float1", 0, 0.f, 10.f, 1.f, 5.f};
    FloatVariableInfo floatInfo2{"float2", 0, 0.f, 10.f, 1.f, 5.f};

    auto pythonProvider = std::make_unique<PythonProvider>("SceneName", "FileName", std::vector<FloatVariableInfo>{floatInfo1, floatInfo2},
                                                           std::vector<EnumVariableInfo>(), nullptr);

    SceneMetadata meta("test", "test");
    Scene scene(meta);
    scene.addNode(SceneNode("node", std::move(pythonProvider), nullptr));

    auto varMap = scene.variableInfoMap();
    ASSERT_EQ(1, varMap.size());
    ASSERT_EQ(2, varMap["node"].floatInfos.size());
    ASSERT_EQ(0, varMap["node"].enumInfos.size());
}

TEST_F(SceneTest, EnumVariables) {
    EnumVariableInfo enumInfo1{"enum1", 0, {"val1", "val2"}, "val2"};
    EnumVariableInfo enumInfo2{"enum2", 0, {"val1", "val2"}, "val2"};

    auto pythonProvider = std::make_unique<PythonProvider>("SceneName", "FileName", std::vector<FloatVariableInfo>(),
                                                           std::vector<EnumVariableInfo>{enumInfo1, enumInfo2}, nullptr);

    SceneMetadata meta("test", "test");
    Scene scene(meta);
    scene.addNode(SceneNode("node", std::move(pythonProvider), nullptr));

    auto varMap = scene.variableInfoMap();
    ASSERT_EQ(1, varMap.size());
    ASSERT_EQ(0, varMap["node"].floatInfos.size());
    ASSERT_EQ(2, varMap["node"].enumInfos.size());
}