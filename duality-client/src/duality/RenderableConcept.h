#pragma once

class RenderDispatcher2D;
class RenderDispatcher3D;

#include <memory>

class Renderable {
    struct RenderableConcept {
        virtual ~RenderableConcept() {}
        virtual void render(RenderDispatcher2D& dispatcher) = 0;
        virtual void render(RenderDispatcher3D& dispatcher) = 0;
    };

    template <typename T> struct RenderableModel : RenderableConcept {
        RenderableModel(const T& t)
            : m_object(t) {}
        virtual ~RenderableModel() {}
        void render(RenderDispatcher2D& dispatcher) override {
            m_object->render(dispatcher);
        }
        void render(RenderDispatcher3D& dispatcher) override {
            m_object->render(dispatcher);
        }

    private:
        T m_object;
    };

    std::unique_ptr<RenderableConcept> m_object;

public:
    template <typename T>
    Renderable(const T& obj)
        : m_object(std::make_unique<RenderableModel<T>>(obj)) {}
    void render(RenderDispatcher2D& dispatcher) {
        m_object->render(dispatcher);
    }
    void render(RenderDispatcher3D& dispatcher) {
        m_object->render(dispatcher);
    }
};