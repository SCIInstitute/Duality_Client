#pragma once

#include "IVDA/Vectors.h"
#include "duality/DatasetDispatcher.h"

#include <memory>
#include <vector>

class Dataset {
public:
    Dataset(std::vector<IVDA::Mat4f> transforms = {});
    virtual ~Dataset();

    virtual void accept(DatasetDispatcher& dispatcher) = 0;

    void load(std::shared_ptr<std::vector<uint8_t>> data);
    std::vector<IVDA::Mat4f> transforms() const;

private:
    virtual void read(std::shared_ptr<std::vector<uint8_t>> data) = 0;
    virtual void applyTransform(const IVDA::Mat4f& matrix) = 0;

private:
    std::vector<IVDA::Mat4f> m_transforms;
};