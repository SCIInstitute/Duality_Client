#include "src/duality/TransferFunction.h"

#include "duality/Error.h"

TransferFunctionData duality::defaultTransferFunctionData() {
    TransferFunctionData tf;
    for (int i = 0; i < 256; ++i) {
        uint8_t rgb = static_cast<uint8_t>(i);
        uint8_t alpha = (i < 50) ? static_cast<uint8_t>(0) : static_cast<uint8_t>(i);
        tf[i] = {rgb, rgb, rgb, alpha};
    }
    return tf;
}

TransferFunction::TransferFunction(std::unique_ptr<DataProvider> provider)
    : m_provider(std::move(provider)) {}

void TransferFunction::fetch() {
    auto data = m_provider->fetch();
    if (data != nullptr) {
        readData(*data);
    } else {
        m_data = duality::defaultTransferFunctionData();
    }
    initTexture();
}

void TransferFunction::bindTexture() const {
    m_texture->bindWithUnit(0);
}

void TransferFunction::readData(const std::vector<uint8_t>& data) {
    if (data.size() != sizeof(TransferFunctionData)) {
        throw Error("Invalid transfer function", __FILE__, __LINE__);
    }
    std::memcpy(m_data.data(), data.data(), data.size());
}

void TransferFunction::initTexture() {
    // apply opacity correction
    TransferFunctionData correctedTf = m_data;
    float quality = 1.0f;
    for (int i = 0; i < 256; i++) {
        double alpha = correctedTf[i][3] / 255.0;
        alpha = 1.0 - std::pow(1.0 - alpha, 1.0 / quality);
        correctedTf[i][3] = static_cast<uint8_t>(255.0 * alpha);
    }
    m_texture = std::make_unique<GLTexture2D>(correctedTf.data(), GLTexture2D::TextureData::Color, 256, 1);
}
