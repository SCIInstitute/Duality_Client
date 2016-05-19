#include "gmock/gmock.h"

#include "Mocks/DataProviderMock.h"
#include "Mocks/DatasetMock.h"
#include "Scene/DownloadProvider.h"
#include "Scene/PythonProvider.h"
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
    EXPECT_CALL(*providerMock1, fetch()).Times(1);
    auto datasetMock1 = std::make_unique<DatasetMock>();
    EXPECT_CALL(*datasetMock1, accept(_)).Times(1);
    scene.addNode(SceneNode("node", std::move(providerMock1), std::move(datasetMock1)));

    auto providerMock2 = std::make_unique<DataProviderMock>();
    EXPECT_CALL(*providerMock2, fetch()).Times(1);
    auto datasetMock2 = std::make_unique<DatasetMock>();
    EXPECT_CALL(*datasetMock2, accept(_)).Times(1);
    scene.addNode(SceneNode("node", std::move(providerMock2), std::move(datasetMock2)));

    scene.updateDatasets();
}

TEST_F(SceneTest, FloatManipulators) {
    InputVariableFloat::Info floatInfo1{"float1", 0, 0.f, 10.f, 1.f, 5.f};
    InputVariableFloat floatVar1(floatInfo1);

    InputVariableFloat::Info floatInfo2{"float2", 0, 0.f, 10.f, 1.f, 5.f};
    InputVariableFloat floatVar2(floatInfo2);

    auto pythonProvider = std::make_unique<PythonProvider>("SceneName", "FileName", std::vector<InputVariableFloat>{floatVar1, floatVar2},
                                                           std::vector<InputVariableEnum>(), nullptr);

    SceneMetadata meta("test", "test");
    Scene scene(meta);
    scene.addNode(SceneNode("node", std::move(pythonProvider), nullptr));

    auto setterMap = scene.variableSetterMap();
    ASSERT_EQ(1, setterMap.size());
    ASSERT_EQ(2, setterMap["node"].floatSetters.size());
    ASSERT_EQ(0, setterMap["node"].enumSetters.size());
}

TEST_F(SceneTest, CreateEnumManipulators) {
    InputVariableEnum::Info enumInfo1{"enum1", 0, {"val1", "val2"}, "val2"};
    InputVariableEnum enumVar1(enumInfo1);

    InputVariableEnum::Info enumInfo2{"enum2", 0, {"val1", "val2"}, "val2"};
    InputVariableEnum enumVar2(enumInfo2);

    auto pythonProvider = std::make_unique<PythonProvider>("SceneName", "FileName", std::vector<InputVariableFloat>(),
                                                           std::vector<InputVariableEnum>{enumVar1, enumVar2}, nullptr);

    SceneMetadata meta("test", "test");
    Scene scene(meta);
    scene.addNode(SceneNode("node", std::move(pythonProvider), nullptr));

    auto setterMap = scene.variableSetterMap();
    ASSERT_EQ(1, setterMap.size());
    ASSERT_EQ(0, setterMap["node"].floatSetters.size());
    ASSERT_EQ(2, setterMap["node"].enumSetters.size());
}