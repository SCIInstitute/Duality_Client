#pragma once

#include "IVDA/GLMatrix.h"
#include "src/duality/VolumeDataset.h"
#include "src/duality/SceneNode.h"
#include "src/duality/View.h"

#include <memory>

class VolumeNode : public SceneNode {
public:
    VolumeNode(const std::string& name, Visibility visibility, std::unique_ptr<VolumeDataset> dataset);
    
    void updateDataset() override;
    void dispatch(DatasetDispatcher& dispatcher) const override;
    std::vector<FloatVariableInfo> floatVariableInfos() const override;
    std::vector<EnumVariableInfo> enumVariableInfos() const override;
    void setVariable(const std::string& variableName, float value) override;
    void setVariable(const std::string& variableName, const std::string& value) override;
    
private:
    std::unique_ptr<VolumeDataset> m_dataset;
};