#include "src/duality/MVP3D.h"

using namespace IVDA;

MVP3D::MVP3D(const ScreenInfo& screenInfo, const BoundingBox& boundingBox) {
    createModelView(boundingBox);
    createProjection(screenInfo);
}

GLMatrix MVP3D::calculate(const RenderParameters3D& parameters) const {
    GLMatrix matrix = m_defaultModelView;
    matrix.multiply((GLMatrix)parameters.rotation());
    matrix.translate(parameters.transation().x, parameters.transation().y, parameters.transation().z);
    matrix.multiply(m_projection);
    return matrix;
}

void MVP3D::updateScreenInfo(const ScreenInfo& screenInfo) {
    createProjection(screenInfo);
}

void MVP3D::updateBoundingBox(const BoundingBox& boundingBox) {
    createModelView(boundingBox);
}

void MVP3D::createModelView(const BoundingBox& boundingBox) {
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
