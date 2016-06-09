#pragma once

#include "IVDA/GLMatrix.h"
#include "duality/ScreenInfo.h"
#include "src/duality/BoundingBox.h"
#include "src/duality/RenderParameters3D.h"

class MVP3D {
public:
    MVP3D() = default;
    MVP3D(const ScreenInfo& screenInfo, const BoundingBox& boundingBox, const RenderParameters3D& parameters);

    void updateParameters(const RenderParameters3D& parameters);
    void updateScreenInfo(const ScreenInfo& screenInfo);
    void updateBoundingBox(const BoundingBox& boundingBox);

    const GLMatrix& mv() const;
    const GLMatrix& mvp() const;

private:
    void createDefaultModelView(const BoundingBox& boudningBox);
    void createProjection(const ScreenInfo& screenInfo);

private:
    GLMatrix m_defaultModelView;
    GLMatrix m_projection;
    GLMatrix m_mv;
    GLMatrix m_mvp;
};