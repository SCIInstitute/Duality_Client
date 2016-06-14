#pragma once

#include "IVDA/Vectors.h"

class RenderParameters3D {
public:
    RenderParameters3D() = default;
    RenderParameters3D(const IVDA::Vec3f& initialTranslation, const IVDA::Mat4f& initialRotation);

    const IVDA::Vec3f& transation() const noexcept;
    const IVDA::Mat4f& rotation() const noexcept;

    void addTranslation(const IVDA::Vec2f& translation);
    void addRotation(const IVDA::Mat4f& rotation);
    void addZoom(const float zoom);

private:
    IVDA::Vec3f m_translation;
    IVDA::Mat4f m_rotation;
};

bool operator==(const RenderParameters3D& lhs, const RenderParameters3D& rhs);
bool operator!=(const RenderParameters3D& lhs, const RenderParameters3D& rhs);