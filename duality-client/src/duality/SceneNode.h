#pragma once

#include "src/duality/View.h"
#include "src/duality/BoundingBox.h"
#include "src/duality/RenderDispatcher2D.h"
#include "src/duality/RenderDispatcher3D.h"

class SceneNode {
public:
    SceneNode(const std::string& name, Visibility visibility);
    virtual ~SceneNode() {}
    
    std::string name() const;
    bool isVisibleInView(View view) const;

    virtual void render(RenderDispatcher2D& dispatcher) = 0;
    virtual void render(RenderDispatcher3D& dispatcher) = 0;
    virtual BoundingBox boundingBox() const = 0;
    virtual void updateDataset() = 0;
    virtual void initializeDataset() = 0;

private:
    std::string m_name;
    Visibility m_visibility;
};