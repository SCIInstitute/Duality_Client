#include "gtest/gtest.h"

#include "Scene/DownloadProvider.h"
#include "Scene/GeometryDataset.h"
#include "Scene/PythonProvider.h"
#include "Scene/SceneParser.h"

class SceneParserTest : public ::testing::Test {
protected:
    SceneParserTest() {}

    virtual ~SceneParserTest() {}
};

TEST_F(SceneParserTest, Metadata) {
    JsonCpp::Value root;
    root["metadata"]["name"] = "my scene";
    root["metadata"]["description"] = "test scene";
    SceneMetadata result = SceneParser::parseMetadata(root);
    ASSERT_EQ("my scene", result.name());
    ASSERT_EQ("test scene", result.description());
}

TEST_F(SceneParserTest, TwoNodes) {
    JsonCpp::Value root;
    root["scene"][0]["dataset"]["type"] = "geometry";
    root["scene"][0]["source"]["type"] = "download";
    root["scene"][0]["source"]["filename"] = "/some/file1";
    root["scene"][1]["dataset"]["type"] = "geometry";
    root["scene"][1]["source"]["type"] = "download";
    root["scene"][1]["source"]["filename"] = "/some/file2";

    SceneParser parser(root, nullptr);
    auto scene = parser.parseScene();
    const auto& nodes = scene->nodes();
    ASSERT_EQ(2, nodes.size());

    const auto& node1 = scene->nodes()[0];
    const auto dataset1 = node1.dataset();
    ASSERT_TRUE(dynamic_cast<const GeometryDataset*>(dataset1) != nullptr);
    const auto dataProvider1 = node1.dataProvider();
    ASSERT_TRUE(dynamic_cast<const DownloadProvider*>(dataProvider1) != nullptr);
    ASSERT_EQ("/some/file1", dynamic_cast<const DownloadProvider*>(dataProvider1)->fileName());

    const auto& node2 = scene->nodes()[1];
    const auto dataset2 = node1.dataset();
    ASSERT_TRUE(dynamic_cast<const GeometryDataset*>(dataset2) != nullptr);
    const auto dataProvider2 = node2.dataProvider();
    ASSERT_TRUE(dynamic_cast<const DownloadProvider*>(dataProvider2) != nullptr);
    ASSERT_EQ("/some/file2", dynamic_cast<const DownloadProvider*>(dataProvider2)->fileName());
}

TEST_F(SceneParserTest, Transforms) {
    JsonCpp::Value root;
    root["scene"][0]["dataset"]["type"] = "geometry";
    for (int i = 0; i < 16; ++i) {
        root["scene"][0]["dataset"]["transforms"][0][i] = i;
    }
    root["scene"][0]["source"]["type"] = "download";
    root["scene"][0]["source"]["path"] = "/some/file";

    SceneParser parser(root, nullptr);
    auto scene = parser.parseScene();
    const auto& node = scene->nodes()[0];
    const auto& dataset = *node.dataset();
    ASSERT_EQ(1, dataset.transforms().size());
    const auto& translation = dataset.transforms().front();

    ASSERT_EQ(0.0f, translation.m11);
    ASSERT_EQ(1.0f, translation.m12);
    ASSERT_EQ(2.0f, translation.m13);
    ASSERT_EQ(3.0f, translation.m14);

    ASSERT_EQ(4.0f, translation.m21);
    ASSERT_EQ(5.0f, translation.m22);
    ASSERT_EQ(6.0f, translation.m23);
    ASSERT_EQ(7.0f, translation.m24);

    ASSERT_EQ(8.0f, translation.m31);
    ASSERT_EQ(9.0f, translation.m32);
    ASSERT_EQ(10.0f, translation.m33);
    ASSERT_EQ(11.0f, translation.m34);

    ASSERT_EQ(12.0f, translation.m41);
    ASSERT_EQ(13.0f, translation.m42);
    ASSERT_EQ(14.0f, translation.m43);
    ASSERT_EQ(15.0f, translation.m44);
}

TEST_F(SceneParserTest, PythonParams) {
    JsonCpp::Value root;
    root["scene"][0]["dataset"]["type"] = "geometry";
    root["scene"][0]["source"]["type"] = "python";
    root["scene"][0]["source"]["filename"] = "myScript.py";
    root["scene"][0]["source"]["variables"][0]["name"] = "myEnum";
    root["scene"][0]["source"]["variables"][0]["type"] = "enum";
    root["scene"][0]["source"]["variables"][0]["values"][0] = "val1";
    root["scene"][0]["source"]["variables"][0]["values"][1] = "val2";
    root["scene"][0]["source"]["variables"][0]["defaultValue"] = "val2";

    root["scene"][0]["source"]["variables"][1]["name"] = "myFloat";
    root["scene"][0]["source"]["variables"][1]["type"] = "float";
    root["scene"][0]["source"]["variables"][1]["lowerBound"] = 5.f;
    root["scene"][0]["source"]["variables"][1]["upperBound"] = 10.f;
    root["scene"][0]["source"]["variables"][1]["stepSize"] = .5f;
    root["scene"][0]["source"]["variables"][1]["defaultValue"] = 7.f;

    SceneParser parser(root, nullptr);
    auto scene = parser.parseScene();
    const auto& nodes = scene->nodes();

    auto& node = scene->nodes()[0];
    const auto& dataset = *node.dataset();
    ASSERT_TRUE(dynamic_cast<const GeometryDataset*>(&dataset) != nullptr);
    auto provider = dynamic_cast<PythonProvider*>(node.dataProvider());
    ASSERT_TRUE(provider != nullptr);
    ASSERT_EQ("myScript.py", provider->fileName());

    auto enumVars = provider->enumVariableInfos();
    ASSERT_EQ(1, enumVars.size());
    auto enumInfo = enumVars[0];
    ASSERT_EQ("myEnum", enumInfo.name);
    ASSERT_EQ(0, enumInfo.index);
    ASSERT_EQ(2, enumInfo.values.size());
    ASSERT_EQ("val1", enumInfo.values[0]);
    ASSERT_EQ("val2", enumInfo.values[1]);
    ASSERT_EQ("val2", enumInfo.defaultValue);

    auto floatVars = provider->floatVariableInfos();
    ASSERT_EQ(1, floatVars.size());
    auto floatInfo = floatVars[0];
    ASSERT_EQ("myFloat", floatInfo.name);
    ASSERT_EQ(1, floatInfo.index);
    ASSERT_EQ(5.f, floatInfo.lowerBound);
    ASSERT_EQ(10.f, floatInfo.upperBound);
    ASSERT_EQ(.5f, floatInfo.stepSize);
    ASSERT_EQ(7.f, floatInfo.defaultValue);
}