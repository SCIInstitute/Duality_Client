#pragma once

#include "IVDA/GLMatrix.h"
#include "duality/ScreenInfo.h"
#include "src/duality/BoundingBox.h"
#include "src/duality/RenderParameters3D.h"

class MVP3D {
public:
    MVP3D(const ScreenInfo& screenInfo, const BoundingBox& boundingBox);
    GLMatrix calculate(const RenderParameters3D& parameters) const;

    void updateScreenInfo(const ScreenInfo& screenInfo);
    void updateBoundingBox(const BoundingBox& boundingBox);

private:
    void createModelView(const BoundingBox& boudningBox);
    void createProjection(const ScreenInfo& screenInfo);

private:
    GLMatrix m_defaultModelView;
    GLMatrix m_projection;
};