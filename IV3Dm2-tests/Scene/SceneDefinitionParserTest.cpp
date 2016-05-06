#include "gtest/gtest.h"

#include "Scene/SceneDefinitionParser.h"
#include "Scene/DownloadNode.h"
#include "Scene/GroupNode.h"

class SceneDefinitionParserTest : public ::testing::Test {
protected:
  SceneDefinitionParserTest() {}

  virtual ~SceneDefinitionParserTest() {}
};

TEST_F(SceneDefinitionParserTest, DownloadNode) {
  JsonCpp::Value root;
  root["name"] = "my scene";
  root["scene"]["type"] = "download";
  root["scene"]["path"] = "/some/file";

  SceneDefinition result = SceneDefinitionParser::parse(root);
  ASSERT_EQ("my scene", result.name());
  const auto& node = result.rootNode();
  ASSERT_TRUE(dynamic_cast<const DownloadNode*>(&node) != nullptr);
  ASSERT_EQ("/some/file", dynamic_cast<const DownloadNode&>(node).path());
}

TEST_F(SceneDefinitionParserTest, GroupNode) {
    JsonCpp::Value root;
    root["name"] = "my scene";
    root["scene"]["type"] = "group";
    root["scene"]["children"][0]["type"] = "download";
    root["scene"]["children"][0]["path"] = "/some/file1";
    root["scene"]["children"][1]["type"] = "download";
    root["scene"]["children"][1]["path"] = "/some/file2";

    SceneDefinition result = SceneDefinitionParser::parse(root);
    const auto& rootNode = result.rootNode();
    ASSERT_TRUE(dynamic_cast<const GroupNode*>(&rootNode) != nullptr);

    const auto& children = dynamic_cast<const GroupNode&>(rootNode).children();
    ASSERT_TRUE(dynamic_cast<const DownloadNode*>(children[0].get()) != nullptr);
    ASSERT_EQ("/some/file1", dynamic_cast<const DownloadNode&>(*children[0]).path());

    ASSERT_TRUE(dynamic_cast<const DownloadNode*>(children[1].get()) != nullptr);
    ASSERT_EQ("/some/file2", dynamic_cast<const DownloadNode&>(*children[1]).path());
}