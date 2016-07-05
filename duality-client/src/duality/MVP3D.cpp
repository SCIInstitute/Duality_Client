#include "src/duality/MVP3D.h"

using namespace IVDA;

MVP3D::MVP3D(const ScreenInfo& screenInfo, const BoundingBox& boundingBox, const RenderParameters3D& parameters) {
    createDefaultModelView(boundingBox);
    createProjection(screenInfo);
    updateParameters(parameters);
}

void MVP3D::updateParameters(const RenderParameters3D& parameters) {
    m_mv = m_defaultModelView;
    m_mv.multiply((GLMatrix)parameters.rotation());
    m_mv.translate(parameters.transation().x, parameters.transation().y, parameters.transation().z);
    m_mvp = m_mv;
    m_mvp.multiply(m_projection);
}

const GLMatrix& MVP3D::mv() const {
    return m_mv;
}

const GLMatrix& MVP3D::mvp() const {
    return m_mvp;
}

void MVP3D::createDefaultModelView(const BoundingBox& boundingBox) {
    Vec3f size = boundingBox.max - boundingBox.min;
    Vec3f center = boundingBox.min + size / 2;

    m_defaultModelView.loadIdentity();
    m_defaultModelView.translate(-center.x, -center.y, -center.z);

    float maxExtend = size.maxVal();
    m_defaultModelView.scale(1.f / maxExtend, 1.f / maxExtend, 1.f / maxExtend);
}

void MVP3D::createProjection(const ScreenInfo& screenInfo) {
    float zNear = 0.01f;
    float zFar = 1000.0f;
    float frustSize = zNear * tanf(3.1415f / 8.0f);
    float aspectRatio = float(screenInfo.height) / float(screenInfo.width);

    m_projection.loadIdentity();
    m_projection.frustum(-frustSize, frustSize, -frustSize * aspectRatio, frustSize * aspectRatio, zNear, zFar);
}

IVDA::Vec3f MVP3D::eyePos() const {
    Vec4f eyePos(0, 0, 0, 1);
    eyePos = eyePos * static_cast<Mat4f>(m_mvp).inverse();
    return eyePos.dehomo();
}