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

TEST_F(SceneParserTest, Transforms) {
    JsonCpp::Value root;
    root["scene"]["type"] = "geo";
    root["scene"]["source"]["type"] = "download";
    root["scene"]["source"]["path"] = "/some/file";
    for (int i = 0; i < 9; ++i) {
        root["scene"]["transforms"]["translation"][i] = i;
    }
    SceneParser parser(root, nullptr);
    auto scene = parser.parseScene();
    const auto& node = scene->rootNode();
    const auto& translation = *std::get<0>(node.transforms());
    
    ASSERT_EQ(0.0f, translation[0][0]);
    ASSERT_EQ(1.0f, translation[0][1]);
    ASSERT_EQ(2.0f, translation[0][2]);

    ASSERT_EQ(3.0f, translation[1][0]);
    ASSERT_EQ(4.0f, translation[1][1]);
    ASSERT_EQ(5.0f, translation[1][2]);

    ASSERT_EQ(6.0f, translation[2][0]);
    ASSERT_EQ(7.0f, translation[2][1]);
    ASSERT_EQ(8.0f, translation[2][2]);
    
    ASSERT_TRUE(std::get<1>(node.transforms()) == nullptr);
    ASSERT_TRUE(std::get<2>(node.transforms()) == nullptr);
}
