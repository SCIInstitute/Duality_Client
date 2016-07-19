#pragma once

#include "IVDA/Vectors.h"
#include "duality/CoordinateSystem.h"
#include "src/duality/SliderParameter.h"

class RenderParameters2D {
public:
    RenderParameters2D();
    RenderParameters2D(const IVDA::Vec2f& initialTranslation, const float initialRotationAngle, const float initialZoom,
                       CoordinateAxis initialAxis, const SliderParameter& initialSliderParameter);

    const IVDA::Vec2f& transation() const noexcept;
    float rotationAngle() const noexcept;
    float zoom() const noexcept;

    void addTranslation(const IVDA::Vec2f& translation);
    void addRotation(const float rotationAngle);
    void addZoom(const float zoom);

    void setSliderParameter(const SliderParameter& sliderParameter);
    SliderParameter sliderParameter() const noexcept;

    void toggleAxis();
    CoordinateAxis axis() const noexcept;

private:
    IVDA::Vec2f m_translation;
    float m_rotationAngle;
    float m_zoom;
    CoordinateAxis m_axis;
    SliderParameter m_sliderParameter;
};

bool operator==(const RenderParameters2D& lhs, const RenderParameters2D& rhs);
bool operator!=(const RenderParameters2D& lhs, const RenderParameters2D& rhs);