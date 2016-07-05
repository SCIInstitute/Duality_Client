#pragma once

#include "IVDA/GLMatrix.h"
#include "src/duality/VolumeDataset.h"
#include "src/duality/SceneNode.h"
#include "src/duality/View.h"
#include "src/duality/TransferFunction.h"

#include <memory>

class VolumeNode : public SceneNode {
public:
    VolumeNode(const std::string& name, Visibility visibility, std::unique_ptr<VolumeDataset> dataset, std::unique_ptr<TransferFunction> tf);

    void accept(NodeDispatcher& dispatcher) override;
    void updateDataset() override;
    
    BoundingBox boundingBox() const override;
    const VolumeDataset& dataset() const;
    const TransferFunction& transferFunction() const;

private:
    std::unique_ptr<VolumeDataset> m_dataset;
    std::unique_ptr<TransferFunction> m_tf;
};