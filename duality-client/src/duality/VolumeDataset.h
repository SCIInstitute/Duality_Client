#pragma once

#include "src/duality/Dataset.h"
#include "src/duality/I3M.h"

class VolumeDataset : public Dataset {
public:
    VolumeDataset(Dataset::Visibility visibility);

    virtual void accept(DatasetDispatcher& dispatcher) override;

private:
    virtual void read(std::shared_ptr<std::vector<uint8_t>> data) override;
    virtual void applyTransform(const IVDA::Mat4f& matrix) override;

private:
    std::unique_ptr<I3M::Volume> m_volume;
};