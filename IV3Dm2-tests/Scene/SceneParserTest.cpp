#include "gtest/gtest.h"

#include "Scene/SceneParser.h"
#include "Scene/GeometryNode.h"
#include "Scene/DownloadProvider.h"
#include "Scene/SCIRunProvider.h"

class SceneParserTest : public ::testing::Test {
protected:
  SceneParserTest() {}

  virtual ~SceneParserTest() {}
};

TEST_F(SceneParserTest, Metadata) {
    JsonCpp::Value root;
    root["metadata"]["name"] = "my scene";
    root["metadata"]["description"] = "test scene";
    SceneParser parser(root, nullptr);
    SceneMetadata result = parser.parseMetadata();
    ASSERT_EQ("my scene", result.name());
    ASSERT_EQ("test scene", result.description());
}

TEST_F(SceneParserTest, TwoGeoNodes) {
  JsonCpp::Value root;
  root["scene"][0]["type"] = "geometry";
  root["scene"][0]["source"]["type"] = "download";
  root["scene"][0]["source"]["path"] = "/some/file1";
  root["scene"][1]["type"] = "geometry";
  root["scene"][1]["source"]["type"] = "download";
  root["scene"][1]["source"]["path"] = "/some/file2";

  SceneParser parser(root, nullptr);
  auto scene = parser.parseScene();
  const auto& nodes = scene->nodes();
  ASSERT_EQ(2, nodes.size());

  const auto& node1 = *scene->nodes()[0];
  ASSERT_TRUE(dynamic_cast<const GeometryNode*>(&node1) != nullptr);
  const auto& dataProvider1 = node1.dataProvider();
  ASSERT_TRUE(dynamic_cast<const DownloadProvider*>(&dataProvider1) != nullptr);
  ASSERT_EQ("/some/file1", dynamic_cast<const DownloadProvider*>(&dataProvider1)->path());

  const auto& node2 = *scene->nodes()[1];
  ASSERT_TRUE(dynamic_cast<const GeometryNode*>(&node2) != nullptr);
  const auto& dataProvider2 = node2.dataProvider();
  ASSERT_TRUE(dynamic_cast<const DownloadProvider*>(&dataProvider2) != nullptr);
  ASSERT_EQ("/some/file2", dynamic_cast<const DownloadProvider*>(&dataProvider2)->path());
}

TEST_F(SceneParserTest, Transforms) {
    JsonCpp::Value root;
    root["scene"][0]["type"] = "geometry";
    root["scene"][0]["source"]["type"] = "download";
    root["scene"][0]["source"]["path"] = "/some/file";
    for (int i = 0; i < 16; ++i) {
        root["scene"][0]["transforms"]["translation"][i] = i;
    }
    SceneParser parser(root, nullptr);
    auto scene = parser.parseScene();
    const auto& node = *scene->nodes()[0];
    ASSERT_EQ(1, node.transforms().size());
    const auto& translation = node.transforms().front();
    
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

TEST_F(SceneParserTest, SCIRunProvider_FloatParams) {
    JsonCpp::Value root;
    root["scene"][0]["type"] = "geometry";
    root["scene"][0]["source"]["type"] = "SCIRun";
    root["scene"][0]["source"]["network"] = "myNetwork";
    root["scene"][0]["source"]["floatParams"][0]["name"] = "myFloat";
    root["scene"][0]["source"]["floatParams"][0]["lowerBound"] = 5.f;
    root["scene"][0]["source"]["floatParams"][0]["upperBound"] = 10.f;
    root["scene"][0]["source"]["floatParams"][0]["stepSize"] = .5f;
    root["scene"][0]["source"]["floatParams"][0]["defaultValue"] = 7.f;

    SceneParser parser(root, nullptr);
    auto scene = parser.parseScene();
    const auto& nodes = scene->nodes();

    const auto& node = *scene->nodes()[0];
    ASSERT_TRUE(dynamic_cast<const GeometryNode*>(&node) != nullptr);
    auto provider = dynamic_cast<const SCIRunProvider*>(&node.dataProvider());
    ASSERT_TRUE(provider != nullptr);
    ASSERT_EQ("myNetwork", provider->network());
    auto floatParams = provider->floatParameters();
    ASSERT_EQ(1, floatParams.size());
    auto floatParam = floatParams[0];
    ASSERT_EQ("myFloat", floatParam.name);
    ASSERT_EQ(5.f, floatParam.lowerBound);
    ASSERT_EQ(10.f, floatParam.upperBound);
    ASSERT_EQ(.5f, floatParam.stepSize);
    ASSERT_EQ(7.f, floatParam.defaultValue);
}

TEST_F(SceneParserTest, SCIRunProvider_EnumParams) {
    JsonCpp::Value root;
    root["scene"][0]["type"] = "geometry";
    root["scene"][0]["source"]["type"] = "SCIRun";
    root["scene"][0]["source"]["network"] = "myNetwork";
    root["scene"][0]["source"]["enumParams"][0]["name"] = "myEnum";
    root["scene"][0]["source"]["enumParams"][0]["values"][0] = "val1";
    root["scene"][0]["source"]["enumParams"][0]["values"][1] = "val2";
    root["scene"][0]["source"]["enumParams"][0]["defaultValue"] = "val2";

    SceneParser parser(root, nullptr);
    auto scene = parser.parseScene();
    const auto& nodes = scene->nodes();

    const auto& node = *scene->nodes()[0];
    ASSERT_TRUE(dynamic_cast<const GeometryNode*>(&node) != nullptr);
    auto provider = dynamic_cast<const SCIRunProvider*>(&node.dataProvider());
    ASSERT_TRUE(provider != nullptr);
    ASSERT_EQ("myNetwork", provider->network());
    auto enumParams = provider->enumParameters();
    ASSERT_EQ(1, enumParams.size());
    auto enumParam = enumParams[0];
    ASSERT_EQ("myEnum", enumParam.name);
    ASSERT_EQ(2, enumParam.values.size());
    ASSERT_EQ("val1", enumParam.values[0]);
    ASSERT_EQ("val2", enumParam.values[1]);
    ASSERT_EQ("val2", enumParam.defaultValue);
}