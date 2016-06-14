#pragma once

#include "src/duality/DatasetDispatcher.h"
#include "src/duality/View.h"

#include "duality/VariableInfo.h"

#include <vector>

class SceneNode {
public:
    SceneNode(const std::string& name, Visibility visibility);

    std::string name() const;
    bool isVisibleInView(View view) const;

    virtual void updateDataset() = 0;
    virtual void dispatch(DatasetDispatcher& dispatcher) const = 0;
    virtual std::vector<FloatVariableInfo> floatVariableInfos() const = 0;
    virtual std::vector<EnumVariableInfo> enumVariableInfos() const = 0;
    virtual void setVariable(const std::string& variableName, float value) = 0;
    virtual void setVariable(const std::string& variableName, const std::string& value) = 0;

private:
    std::string m_name;
    Visibility m_visibility;
};