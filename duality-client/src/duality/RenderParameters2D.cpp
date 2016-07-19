#include "src/duality/RenderParameters2D.h"

RenderParameters2D::RenderParameters2D()
    : m_translation()
    , m_rotationAngle(0.0f)
    , m_zoom(0.0f)
    , m_axis(CoordinateAxis::X_Axis)
    , m_sliderParameter(0, 0.0f) {}

RenderParameters2D::RenderParameters2D(const IVDA::Vec2f& initialTranslation, const float initialRotationAngle, const float initialZoom,
                                       CoordinateAxis initialAxis, const SliderParameter& initialSliderParameter)
    : m_translation(initialTranslation)
    , m_rotationAngle(initialRotationAngle)
    , m_zoom(initialZoom)
    , m_axis(initialAxis)
    , m_sliderParameter(initialSliderParameter) {}

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
    IVDA::Vec2f o = translation * 2.0f / m_zoom;
    float c = cos(-m_rotationAngle);
    float s = sin(-m_rotationAngle);
    const float xOffset = o.x * c - o.y * s;
    const float yOffset = o.y * c + o.x * s;
    m_translation += IVDA::Vec2f(xOffset, yOffset);
}

void RenderParameters2D::addRotation(const float rotationAngle) {
    if (rotationAngle > 1) {
        return;
    }
    m_rotationAngle += rotationAngle;
}

void RenderParameters2D::addZoom(const float zoom) {
    m_zoom += (m_zoom * zoom);
}

void RenderParameters2D::setSliderParameter(const SliderParameter& sliderParameter) {
    m_sliderParameter = sliderParameter;
}

SliderParameter RenderParameters2D::sliderParameter() const noexcept {
    return m_sliderParameter;
}

void RenderParameters2D::toggleAxis() {
    m_axis = static_cast<CoordinateAxis>((m_axis + 1) % 3);
}

CoordinateAxis RenderParameters2D::axis() const noexcept {
    return m_axis;
}

bool operator==(const RenderParameters2D& lhs, const RenderParameters2D& rhs) {
    return lhs.transation() == rhs.transation() && lhs.rotationAngle() == rhs.rotationAngle() && lhs.zoom() == rhs.zoom() &&
           lhs.axis() == rhs.axis() && lhs.sliderParameter() == rhs.sliderParameter();
}

bool operator!=(const RenderParameters2D& lhs, const RenderParameters2D& rhs) {
    return !(lhs == rhs);
}