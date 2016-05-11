//  Created by David McCann on 5/5/16.
//  Copyright © 2016 Scientific Computing and Imaging Institute. All rights reserved.
//

#include "Scene/Scene.h"

#include "IVDA/Vectors.h"
#include "Scene/AbstractDispatcher.h"
#include "Scene/BoundingBoxCalculator.h"
#include "Scene/SceneParser.h"

using namespace IVDA;

Scene::Scene(SceneMetadata metadata)
    : m_metadata(std::move(metadata)) {}

SceneMetadata Scene::metadata() const noexcept {
    return m_metadata;
}

void Scene::addNode(std::unique_ptr<SceneNode> node) {
    m_nodes.push_back(std::move(node));
}

const std::vector<std::unique_ptr<SceneNode>>& Scene::nodes() const noexcept {
    return m_nodes;
}

void Scene::updateDatasets() {
    for (auto& node : m_nodes) {
        node->updateDataset();
    }
    m_modelView = defaultModelView();
}

void Scene::dispatch(AbstractDispatcher& dispatcher) const {
    for (auto& node : m_nodes) {
        node->accept(dispatcher);
    }
}

GLMatrix Scene::modelViewMatrix() const {
    return m_modelView;
}

GLMatrix Scene::defaultModelView() const {
    BoundingBoxCalculator bbCalc;
    dispatch(bbCalc);

    auto minMax = bbCalc.getMinMax();
    Vec3f vMin = minMax.first;
    Vec3f vMax = minMax.second;

    Vec3f size = vMax - vMin;
    Vec3f center = vMin + size / 2;

    GLMatrix result;
    result.loadIdentity();
    result.translate(-center.x, -center.y, -center.z);

    float maxExtend = size.maxVal();
    result.scale(1.f / maxExtend, 1.f / maxExtend, 1.f / maxExtend);

    return result;
}
