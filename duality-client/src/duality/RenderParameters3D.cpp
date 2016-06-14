#include "src/duality/RenderParameters3D.h"

RenderParameters3D::RenderParameters3D(const IVDA::Vec3f& initialTranslation, const IVDA::Mat4f& initialRotation)
    : m_translation(initialTranslation)
    , m_rotation(initialRotation) {}

const IVDA::Vec3f& RenderParameters3D::transation() const noexcept {
    return m_translation;
}

const IVDA::Mat4f& RenderParameters3D::rotation() const noexcept {
    return m_rotation;
}

void RenderParameters3D::addTranslation(const IVDA::Vec2f& translation) {
    m_translation.x += translation.x;
    m_translation.y += translation.y;
}

void RenderParameters3D::addRotation(const IVDA::Mat4f& rotation) {
    m_rotation = m_rotation * rotation;
}

void RenderParameters3D::addZoom(const float zoom) {
    m_translation.z += zoom;
}

bool operator==(const RenderParameters3D& lhs, const RenderParameters3D& rhs) {
    return lhs.transation() == rhs.transation() && lhs.rotation() == rhs.rotation();
}

bool operator!=(const RenderParameters3D& lhs, const RenderParameters3D& rhs) {
    return !(lhs == rhs);
}