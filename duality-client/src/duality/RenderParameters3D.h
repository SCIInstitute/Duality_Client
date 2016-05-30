#pragma once

#include "IVDA/Vectors.h"

class RenderParameters3D {
public:
    RenderParameters3D(const IVDA::Vec3f& initialTranslation, const IVDA::Mat4f& initialRotation);

    const IVDA::Vec3f& transation() const noexcept;
    const IVDA::Mat4f& rotation() const noexcept;

    void addTranslation(const IVDA::Vec2f& translation);
    void addRotation(const IVDA::Mat4f& rotation);

private:
    IVDA::Vec3f m_translation;
    IVDA::Mat4f m_rotation;
};