#include "src/duality/Scene.h"

#include "IVDA/Vectors.h"
#include "src/duality/GeometryNode.h"
#include "src/duality/SceneParser.h"
#include "src/duality/View.h"
#include "src/duality/VolumeNode.h"

#include "mocca/base/ContainerTools.h"

using namespace IVDA;

Scene::Scene(SceneMetadata metadata, std::vector<std::unique_ptr<SceneNode>> nodes,
             std::map<std::string, std::shared_ptr<Variables>> variables, const std::string& webViewURL)
    : m_metadata(std::move(metadata))
    , m_nodes(std::move(nodes))
    , m_variables(std::move(variables))
    , m_webViewURL(webViewURL) {}

SceneMetadata Scene::metadata() const {
    return m_metadata;
}

std::string Scene::webViewURL() const {
    return m_webViewURL;
}

const std::vector<std::unique_ptr<SceneNode>>& Scene::nodes() const {
    return m_nodes;
}

std::vector<const GeometryNode*> Scene::geometryNodes() const {
    std::vector<const GeometryNode*> nodes;
    for (const auto& node : m_nodes) {
        const auto geo = dynamic_cast<const GeometryNode*>(node.get());
        if (geo != nullptr) {
            nodes.push_back(geo);
        }
    }
    return nodes;
}

std::vector<const VolumeNode*> Scene::volumeNodes() const {
    std::vector<const VolumeNode*> nodes;
    for (const auto& node : m_nodes) {
        const auto geo = dynamic_cast<const VolumeNode*>(node.get());
        if (geo != nullptr) {
            nodes.push_back(geo);
        }
    }
    return nodes;
}

BoundingBox Scene::boundingBox(View view) const {
    IVDA::Vec3f vMin(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
    IVDA::Vec3f vMax(-std::numeric_limits<float>::max(), -std::numeric_limits<float>::max(), -std::numeric_limits<float>::max());
    for (const auto& node : m_nodes) {
        if (node->isVisibleInView(view)) {
            BoundingBox bb = node->boundingBox();
            vMin.StoreMin(bb.min);
            vMax.StoreMax(bb.max);
        }
    }
    return BoundingBox{vMin, vMax};
}

void Scene::render(RenderDispatcher2D& dispatcher) const {
    dispatcher.render(m_nodes);
}

void Scene::render(RenderDispatcher3D& dispatcher) const {
    dispatcher.render(m_nodes);
}

void Scene::updateDatasets() {
    for (auto& node : m_nodes) {
        node->updateDataset();
    }
}

VariableMap Scene::variableMap(View view) {
    VariableMap result;
    for (auto& node : m_nodes) {
        if (node->isVisibleInView(view)) {
            auto name = node->name();
            for (const auto& var : m_variables[name]->floatVariables) {
                result[name].floatVariables.push_back(var);
            }
            for (const auto& var : m_variables[name]->enumVariables) {
                result[name].enumVariables.push_back(var);
            }
        }
    }
    return result;
}

void Scene::setVariable(const std::string& objectName, const std::string& variableName, float value) {
    assert(m_variables.count(objectName) != 0);
    auto& vars = m_variables[objectName]->floatVariables;
    auto it = mocca::findMemberEqual(begin(vars), end(vars), &FloatVariable::name, variableName);
    assert(it != end(vars));
    it->value = value;
}

void Scene::setVariable(const std::string& objectName, const std::string& variableName, const std::string& value) {
    assert(m_variables.count(objectName) != 0);
    auto& vars = m_variables[objectName]->enumVariables;
    auto it = mocca::findMemberEqual(begin(vars), end(vars), &EnumVariable::name, variableName);
    assert(it != end(vars));
    it->value = value;
}