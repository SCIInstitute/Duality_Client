//  Created by David McCann on 5/5/16.
//  Copyright © 2016 Scientific Computing and Imaging Institute. All rights reserved.
//

#include "Scene/Scene.h"

#include "IVDA/Vectors.h"
#include "Scene/BoundingBoxCalculator.h"
#include "Scene/SceneParser.h"

using namespace IVDA;

Scene::Scene(SceneMetadata metadata)
    : m_metadata(std::move(metadata))
    , m_translation(Vec3f(0.f, 0.f, -3.f)) {}

SceneMetadata Scene::metadata() const {
    return m_metadata;
}

void Scene::addNode(SceneNode node) {
    m_nodes.push_back(std::move(node));
}

const std::vector<SceneNode>& Scene::nodes() const {
    return m_nodes;
}

std::vector<SceneNode>& Scene::nodes() {
    return m_nodes;
}

void Scene::dispatch(DatasetDispatcher& dispatcher) const {
    for (auto& node : m_nodes) {
        node.dispatch(dispatcher);
    }
}

void Scene::updateDatasets() {
    for (auto& node : m_nodes) {
        node.updateDataset();
    }
    m_defaultModelView = defaultModelView();
}

Scene::VariableSetterMap Scene::variableSetterMap() {
    VariableSetterMap result;
    for (auto& node : m_nodes) {
        result[node.name()].floatSetters = node.dataProvider()->floatSetters();
        result[node.name()].enumSetters = node.dataProvider()->enumSetters();
    }
    return result;
}

void Scene::addTranslation(const IVDA::Vec2f& translation) {
    m_translation.x += translation.x;
    m_translation.y += translation.y;
}

void Scene::addRotation(const IVDA::Mat4f& rotation) {
    m_rotation = m_rotation * rotation;
}

GLMatrix Scene::modelViewMatrix() const {
    GLMatrix modelView = m_defaultModelView;
    modelView.multiply((GLMatrix)m_rotation);
    modelView.translate(m_translation.x, m_translation.y, m_translation.z);
    return modelView;
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
