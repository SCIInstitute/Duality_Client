#include "src/duality/SliderParameterCalculator.h"

#include "src/duality/GeometryNode.h"
#include "src/duality/Scene.h"
#include "src/duality/VolumeNode.h"

#include <algorithm>
#include <cassert>

SliderParameterCalculator::SliderParameterCalculator(const Scene& scene) {
    auto boundingBox = scene.boundingBox(View::View2D);
    m_bounds[0] = std::make_pair(boundingBox.min[0], boundingBox.max[0]);
    m_bounds[1] = std::make_pair(boundingBox.min[1], boundingBox.max[1]);
    m_bounds[2] = std::make_pair(boundingBox.min[2], boundingBox.max[2]);

    auto volumeNodes = scene.volumeNodes();
    if (!volumeNodes.empty()) {
        const auto& firstSliceInfos = volumeNodes.front()->dataset().sliceInfos();
        std::array<int, 3> firstNumSlices{static_cast<int>(firstSliceInfos[0].size()), static_cast<int>(firstSliceInfos[1].size()),
                                          static_cast<int>(firstSliceInfos[2].size())};

        auto hasMatchingNumSlices = [&](const VolumeNode* v) {
            const auto& sliceInfos = v->dataset().sliceInfos();
            std::array<int, 3> numSlices{static_cast<int>(sliceInfos[0].size()), static_cast<int>(sliceInfos[1].size()),
                                         static_cast<int>(sliceInfos[2].size())};
            return numSlices == firstNumSlices;
        };

        auto hasMatchingBoundingBox = [&](const VolumeNode* v) { return v->boundingBox() == boundingBox; };

        if (std::all_of(begin(volumeNodes), end(volumeNodes), hasMatchingNumSlices) &&
            std::all_of(begin(volumeNodes), end(volumeNodes), hasMatchingBoundingBox)) {
            m_supportsSlice = true;
            m_numSlices = firstNumSlices;
        }
    } else {
        m_supportsSlice = false;
    }
}

bool SliderParameterCalculator::supportsSlice() const {
    return m_supportsSlice;
}

SliderParameter SliderParameterCalculator::parameterForSlice(int slice, CoordinateAxis axis) const {
    assert(m_supportsSlice);
    float relative = static_cast<float>(slice) / static_cast<float>(m_numSlices[axis]);
    float range = std::abs(m_bounds[axis].second - m_bounds[axis].first);
    float depth = m_bounds[axis].first + relative * range;
    return SliderParameter(slice, depth);
}

SliderParameter SliderParameterCalculator::parameterForDepth(float depth, CoordinateAxis axis) const {
    if (m_supportsSlice) {
        float range = std::abs(m_bounds[axis].second - m_bounds[axis].first);
        int slice = std::min<int>((depth - m_bounds[axis].first) / range * m_numSlices[axis], m_numSlices[axis] - 1);
        return SliderParameter(slice, depth);
    }
    return SliderParameter(depth);
}

int SliderParameterCalculator::numSlicesForAxis(CoordinateAxis axis) const {
    return m_numSlices[axis];
}

std::pair<float, float> SliderParameterCalculator::boundsForAxis(CoordinateAxis axis) const {
    return m_bounds[axis];
}
