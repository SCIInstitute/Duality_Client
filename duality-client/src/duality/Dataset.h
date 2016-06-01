#pragma once

#include "IVDA/Vectors.h"
#include "src/duality/DatasetDispatcher.h"
#include "src/duality/View.h"

#include <memory>
#include <vector>

class Dataset {
public:
    enum class Visibility : int { VisibleNone = 0, Visible2D = 1, Visible3D = 2, VisibleBoth = 3 };
    Dataset(std::vector<IVDA::Mat4f> transforms = {}, Visibility visibility = Visibility::VisibleBoth);
    virtual ~Dataset();

    virtual void accept(DatasetDispatcher& dispatcher) = 0;

    void load(std::shared_ptr<std::vector<uint8_t>> data);
    std::vector<IVDA::Mat4f> transforms() const;

    bool isVisibleInView(View view) const;
    
private:
    virtual void read(std::shared_ptr<std::vector<uint8_t>> data) = 0;
    virtual void applyTransform(const IVDA::Mat4f& matrix) = 0;

private:
    std::vector<IVDA::Mat4f> m_transforms;
    Visibility m_visibility;
};