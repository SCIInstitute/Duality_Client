#pragma once

#include "src/duality/NodeDispatcher.h"
#include "src/duality/View.h"

#include "duality/InputVariable.h"

#include <vector>

class SceneNode {
public:
    SceneNode(const std::string& name, Visibility visibility);
    virtual ~SceneNode() {}
    
    std::string name() const;
    bool isVisibleInView(View view) const;

    virtual void accept(NodeDispatcher& dispatcher) = 0;
    virtual void updateDataset() = 0;

private:
    std::string m_name;
    Visibility m_visibility;
};