//  Created by David McCann on 5/5/16.
//  Copyright © 2016 Scientific Computing and Imaging Institute. All rights reserved.
//

#include "Scene/Scene.h"

#include "IVDA/Vectors.h"
#include "Scene/BoundingBoxCalculator.h"
#include "Scene/SceneParser.h"
#include "Scene/UpdateDataDispatcher.h"

using namespace IVDA;

Scene::Scene(SceneMetadata metadata)
    : m_metadata(std::move(metadata)) {}

SceneMetadata Scene::metadata() const {
    return m_metadata;
}

void Scene::addNode(std::unique_ptr<SceneNode> node) {
    m_nodes.push_back(std::move(node));
}

const std::vector<std::unique_ptr<SceneNode>>& Scene::nodes() const {
    return m_nodes;
}

void Scene::updateDatasets() {
    UpdateDataDispatcher dispatcher;
    for (auto& node : m_nodes) {
        node->updateDatasets(dispatcher);
    }
    m_defaultModelView = defaultModelView();
}

void Scene::render(RenderDispatcher& dispatcher) const {
    dispatcher.setup();
    for (auto& node : m_nodes) {
        node->render(dispatcher);
    }
    dispatcher.finish();
}

std::vector<ParameterManipulator*> Scene::parameterManipulators() const {
    ParameterManipulatorCollector dispatcher;
    for (auto& node : m_nodes) {
        node->makeManipultor(dispatcher);
    }
    return dispatcher.manipulators();
}

void Scene::addTranslation(const IVDA::Vec2f& translation) {
    m_translation.x += translation.x;
    m_translation.y += translation.y;
}

GLMatrix Scene::modelViewMatrix() const {
    GLMatrix modelView = m_defaultModelView;
    modelView.translate(m_translation.x, m_translation.y, m_translation.z);
    return modelView;
}

GLMatrix Scene::defaultModelView() const {
    BoundingBoxCalculator bbCalc;
    for (auto& node : m_nodes) {
        node->calculateBoundingBox(bbCalc);
    }

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

    // FIXME: just a temporary solution
    result.translate(0, 0, -3.f);

    return result;
}
