#include "duality/SceneMetadata.h"

#include "src/duality/SceneParser.h"

SceneMetadata::SceneMetadata(std::string name, std::string description, std::string url)
    : m_name(std::move(name))
    , m_description(std::move(description))
    , m_url(std::move(url)) {}

std::string SceneMetadata::name() const {
    return m_name;
}

std::string SceneMetadata::description() const {
    return m_description;
}

std::string SceneMetadata::url() const {
    return m_url;
}