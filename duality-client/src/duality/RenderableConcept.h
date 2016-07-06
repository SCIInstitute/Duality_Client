#pragma once

#include "src/duality/BoundingBox.h"

#include <memory>
#include <type_traits>

class RenderDispatcher2D;
class RenderDispatcher3D;

class Renderable {
    struct RenderableConcept {
        virtual ~RenderableConcept() {}
        virtual void render(RenderDispatcher3D& dispatcher) = 0;
        virtual BoundingBox boundingBox() const = 0;
    };

    template <typename T> struct RenderableModel : RenderableConcept {
        RenderableModel(const T& t)
            : m_object(t) {}
        virtual ~RenderableModel() {}
        void render(RenderDispatcher3D& dispatcher) override { renderImpl(dispatcher, std::is_pointer<T>()); }
        BoundingBox boundingBox() const override { return boundingBoxImpl(std::is_pointer<T>()); }
        
    private:
        void renderImpl(RenderDispatcher3D& dispatcher, std::true_type) { m_object->render(dispatcher); }
        void renderImpl(RenderDispatcher3D& dispatcher, std::false_type) { m_object.render(dispatcher); }
        BoundingBox boundingBoxImpl(std::true_type) const { return m_object->boundingBox(); }
        BoundingBox boundingBoxImpl(std::false_type) const { return m_object.boundingBox(); }

        T m_object;
    };

    std::shared_ptr<RenderableConcept> m_object;

public:
    template <typename T>
    Renderable(const T& obj)
        : m_object(std::make_shared<RenderableModel<T>>(obj)) {}
    void render(RenderDispatcher3D& dispatcher) { m_object->render(dispatcher); }
    BoundingBox boundingBox() const { return m_object->boundingBox(); }
};