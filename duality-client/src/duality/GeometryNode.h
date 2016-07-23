#pragma once

#include "IVDA/GLMatrix.h"
#include "src/duality/GeometryDataset.h"
#include "src/duality/SceneNode.h"
#include "src/duality/View.h"

#include <memory>

class GeometryNode : public SceneNode {
public:
    GeometryNode(const std::string& name, Visibility visibility, std::unique_ptr<GeometryDataset> dataset);

    void render(RenderDispatcher2D& dispatcher) override;
    void render(RenderDispatcher3D& dispatcher) override;
    void updateDataset() override;
    void initializeDataset() override;

    BoundingBox boundingBox() const override;
    bool intersects(const BoundingBox& box) const;
    const GeometryDataset& dataset() const;

    bool isTransparent() const;
    
private:
    std::unique_ptr<GeometryDataset> m_dataset;
};