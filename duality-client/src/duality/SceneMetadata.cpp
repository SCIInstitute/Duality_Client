#include "duality/SceneMetadata.h"

#include "src/duality/SceneParser.h"

SceneMetadata::SceneMetadata(std::string name, std::string description)
    : m_name(std::move(name))
    , m_description(std::move(description)) {}

std::string SceneMetadata::name() const {
    return m_name;
}

std::string SceneMetadata::description() const {
    return m_description;
}
