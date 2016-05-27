#include "duality/MVP3D.h"

using namespace IVDA;

MVP3D::MVP3D(const ScreenInfo& screenInfo, const BoundingBox& boundingBox)
    : m_translation(Vec3f(0.f, 0.f, -3.f)) {
    // create default model view matrix

    Vec3f size = boundingBox.max - boundingBox.min;
    Vec3f center = boundingBox.min + size / 2;

    m_defaultModelView.loadIdentity();
    m_defaultModelView.translate(-center.x, -center.y, -center.z);

    float maxExtend = size.maxVal();
    m_defaultModelView.scale(1.f / maxExtend, 1.f / maxExtend, 1.f / maxExtend);

    // create projection matrix
    float zNear = 0.01f;
    float zFar = 1000.0f;
    float frustSize = zNear * tanf(3.1415f / 8.0f);
    float aspectRatio = float(screenInfo.height) / float(screenInfo.width);

    m_projection.loadIdentity();
    m_projection.frustum(-frustSize, frustSize, -frustSize * aspectRatio, frustSize * aspectRatio, zNear, zFar);
}

void MVP3D::addTranslation(const IVDA::Vec2f& translation) {
    m_translation.x += translation.x;
    m_translation.y += translation.y;
}

void MVP3D::addRotation(const IVDA::Mat4f& rotation) {
    m_rotation = m_rotation * rotation;
}

GLMatrix MVP3D::calculate() const {
    GLMatrix matrix = m_defaultModelView;
    matrix.multiply((GLMatrix)m_rotation);
    matrix.translate(m_translation.x, m_translation.y, m_translation.z);
    matrix.multiply(m_projection);
    return matrix;
}