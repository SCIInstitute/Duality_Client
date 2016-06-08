#include "src/duality/AbstractIO.h"
#include "src/duality/VolumeDataset.h"

VolumeDataset::VolumeDataset(Dataset::Visibility visibility)
    : Dataset({}, visibility) {}

void VolumeDataset::accept(DatasetDispatcher& dispatcher) {
    dispatcher.dispatch(*this);
}

void VolumeDataset::read(std::shared_ptr<std::vector<uint8_t>> data) {
    ReaderFromMemory reader(reinterpret_cast<const char*>(data->data()), data->size());
    m_volume = std::make_unique<I3M::Volume>();
    I3M::read(reader, *m_volume);
}

void VolumeDataset::applyTransform(const IVDA::Mat4f& matrix) {
    // FIXME: how to apply transforms to volumetric dataset?
}
