#pragma once

#include "IVDA/Vectors.h"
#include "duality/CoordinateSystem.h"

class RenderParameters2D {
public:
    RenderParameters2D(const IVDA::Vec2f& initialTranslation, const float initialRotationAngle, CoordinateAxis initialAxis);

    const IVDA::Vec2f& transation() const noexcept;
    float rotationAngle() const noexcept;

    void addTranslation(const IVDA::Vec2f& translation);
    void addRotation(float rotationAngle);

    void setSlice(float slice);
    float slice() const noexcept;

    void toggleAxis();
    CoordinateAxis axis() const noexcept;

private:
    IVDA::Vec2f m_translation;
    float m_rotationAngle;
    CoordinateAxis m_axis;
    float m_slice;
};