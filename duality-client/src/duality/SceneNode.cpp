#include "src/duality/SceneNode.h"

SceneNode::SceneNode(const std::string& name, Visibility visibility)
    : m_name(name)
    , m_visibility(visibility) {}

std::string SceneNode::name() const {
    return m_name;
}

bool SceneNode::isVisibleInView(View view) const {
    return (static_cast<int>(m_visibility) & static_cast<int>(view)) != 0;
}
