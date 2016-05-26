#include "duality/MVP2D.h"

#include "duality/CoordinateSystem.h"

MVP2D::MVP2D(const ScreenInfo& screenInfo, const std::pair<IVDA::Vec3f, IVDA::Vec3f>& boundingBox)
    : m_boundingBox(boundingBox)
    , m_translation(0.0f, 0.0f)
    , m_rotationAngle(0.0f) {}

void MVP2D::addTranslation(const IVDA::Vec2f& translation) {}

void MVP2D::addRotation(float angle) {}

GLMatrix MVP2D::calculate() const {
    GLMatrix m;
    return m;
}
