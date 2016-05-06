#include "gtest/gtest.h"

#include "Scene/SceneParser.h"
#include "Scene/DownloadNode.h"
#include "Scene/GroupNode.h"

class SceneParserTest : public ::testing::Test {
protected:
  SceneParserTest() {}

  virtual ~SceneParserTest() {}
};

TEST_F(SceneParserTest, Metadata) {
    JsonCpp::Value root;
    root["name"] = "my scene";
    root["description"] = "test scene";
    SceneMetadata result = SceneParser::parseMetadata(root);
    ASSERT_EQ("my scene", result.name());
    ASSERT_EQ("test scene", result.description());
}

TEST_F(SceneParserTest, DownloadNode) {
  JsonCpp::Value root;
  root["scene"]["type"] = "download";
  root["scene"]["path"] = "/some/file";

  auto result = SceneParser::parseScene(root);
  const auto& node = result->rootNode();
  ASSERT_TRUE(dynamic_cast<const DownloadNode*>(&node) != nullptr);
  ASSERT_EQ("/some/file", dynamic_cast<const DownloadNode&>(node).path());
}

TEST_F(SceneParserTest, GroupNode) {
    JsonCpp::Value root;
    root["scene"]["type"] = "group";
    root["scene"]["children"][0]["type"] = "download";
    root["scene"]["children"][0]["path"] = "/some/file1";
    root["scene"]["children"][1]["type"] = "download";
    root["scene"]["children"][1]["path"] = "/some/file2";

    auto result = SceneParser::parseScene(root);
    const auto& rootNode = result->rootNode();
    ASSERT_TRUE(dynamic_cast<const GroupNode*>(&rootNode) != nullptr);

    const auto& children = dynamic_cast<const GroupNode&>(rootNode).children();
    ASSERT_TRUE(dynamic_cast<const DownloadNode*>(children[0].get()) != nullptr);
    ASSERT_EQ("/some/file1", dynamic_cast<const DownloadNode&>(*children[0]).path());

    ASSERT_TRUE(dynamic_cast<const DownloadNode*>(children[1].get()) != nullptr);
    ASSERT_EQ("/some/file2", dynamic_cast<const DownloadNode&>(*children[1]).path());
}