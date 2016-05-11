#include "gtest/gtest.h"

#include "Scene/SceneParser.h"
#include "Scene/GroupNode.h"
#include "Scene/GeometryNode.h"
#include "Scene/DownloadProvider.h"

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

TEST_F(SceneParserTest, SingleGeoNode) {
  JsonCpp::Value root;
  root["scene"]["type"] = "geo";
  root["scene"]["source"]["type"] = "download";
  root["scene"]["source"]["path"] = "/some/file";

  SceneParser parser(root, nullptr);
  auto scene = parser.parseScene();
  const auto& node = scene->rootNode();
  ASSERT_TRUE(dynamic_cast<const GeometryNode*>(&node) != nullptr);
  const auto& dataProvider = node.dataProvider();
  ASSERT_TRUE(dynamic_cast<const DownloadProvider*>(&dataProvider) != nullptr);
  ASSERT_EQ("/some/file", dynamic_cast<const DownloadProvider*>(&dataProvider)->path());
}

TEST_F(SceneParserTest, GroupNode) {
    JsonCpp::Value root;
    root["scene"]["type"] = "group";
    root["scene"]["children"][0]["type"] = "geo";
    root["scene"]["children"][0]["source"]["type"] = "download";
    root["scene"]["children"][0]["source"]["path"] = "/some/file1";
    root["scene"]["children"][1]["type"] = "geo";
    root["scene"]["children"][1]["source"]["type"] = "download";
    root["scene"]["children"][1]["source"]["path"] = "/some/file2";

    SceneParser parser(root, nullptr);
    auto scene = parser.parseScene();
    const auto& rootNode = scene->rootNode();
    ASSERT_TRUE(dynamic_cast<const GroupNode*>(&rootNode) != nullptr);
    const auto& children = dynamic_cast<const GroupNode&>(rootNode).children();
    ASSERT_EQ(2, children.size());
    ASSERT_TRUE(dynamic_cast<const GeometryNode*>(children[0].get()) != nullptr);
    ASSERT_TRUE(dynamic_cast<const GeometryNode*>(children[1].get()) != nullptr);
}