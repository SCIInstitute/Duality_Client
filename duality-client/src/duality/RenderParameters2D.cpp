#include "src/duality/RenderParameters2D.h"


RenderParameters2D::RenderParameters2D(const IVDA::Vec2f& initialTranslation, const float initialRotationAngle, const float initialZoom,
                                       CoordinateAxis initialAxis)
    : m_translation(initialTranslation)
    , m_rotationAngle(initialRotationAngle)
    , m_zoom(initialZoom)
    , m_axis(initialAxis) {}

const IVDA::Vec2f& RenderParameters2D::transation() const noexcept {
    return m_translation;
}

float RenderParameters2D::rotationAngle() const noexcept {
    return m_rotationAngle;
}

float RenderParameters2D::zoom() const noexcept {
    return m_zoom;
}

void RenderParameters2D::addTranslation(const IVDA::Vec2f& translation) {
    // TODO: implement
}

void RenderParameters2D::addRotation(const float rotationAngle) {
    // TODO:implement
}

void RenderParameters2D::addZoom(const float zoom) {
    m_zoom += (m_zoom * zoom);
}

void RenderParameters2D::setSlice(float slice) {
    m_slice = slice;
}

float RenderParameters2D::slice() const noexcept {
    return m_slice;
}

void RenderParameters2D::toggleAxis() {
    m_axis = static_cast<CoordinateAxis>((m_axis + 1) % 3);
}

CoordinateAxis RenderParameters2D::axis() const noexcept {
    return m_axis;
}
