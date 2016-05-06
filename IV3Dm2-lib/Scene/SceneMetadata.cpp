#include "SceneMetadata.h"

#include "SceneParser.h"

SceneMetadata::SceneMetadata(std::string name, std::string description)
    : m_name(std::move(name))
    , m_description(std::move(description)) {}

SceneMetadata SceneMetadata::fromJson(const JsonCpp::Value& root) {
    return SceneParser::parseMetadata(root);
}

std::string SceneMetadata::name() const {
    return m_name;
}

std::string SceneMetadata::description() const {
    return m_description;
}
