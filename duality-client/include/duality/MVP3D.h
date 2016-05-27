#pragma once

#include "IVDA/Vectors.h"
#include "duality/GLMatrix.h"
#include "duality/ScreenInfo.h"
#include "duality/BoundingBox.h"

class MVP3D {
public:
    MVP3D(const ScreenInfo& screenInfo, const BoundingBox& boundingBox);
    
    void addTranslation(const IVDA::Vec2f& translation);
    void addRotation(const IVDA::Mat4f& rotation);

    GLMatrix calculate() const;
    
private:
    GLMatrix m_defaultModelView;
    GLMatrix m_projection;
    IVDA::Vec3f m_translation;
    IVDA::Mat4f m_rotation;
};