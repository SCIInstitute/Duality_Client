#pragma once

#include "IVDA/Vectors.h"
#include "duality/GLMatrix.h"
#include "duality/ScreenInfo.h"

class MVP2D {
public:
    MVP2D(const ScreenInfo& screenInfo, const std::pair<IVDA::Vec3f, IVDA::Vec3f>& boundingBox);
    
    void addTranslation(const IVDA::Vec2f& translation);
    void addRotation(float angle);
    
    GLMatrix calculate() const;
    
private:
    std::pair<IVDA::Vec3f, IVDA::Vec3f> m_boundingBox;
    IVDA::Vec2f m_translation;
    float m_rotationAngle;
};