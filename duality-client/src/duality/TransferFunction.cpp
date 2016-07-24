#include "src/duality/TransferFunction.h"

#include "duality/Error.h"

#include "mocca/base/StringTools.h"

#include <cmath>
#include <string>

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
    : m_provider(std::move(provider))
    , m_initRequired(true) {}

void TransferFunction::update() {
    if (m_provider != nullptr) {
        auto data = m_provider->fetch();
        if (data != nullptr) {
            readData(*data);
            m_initRequired = true;
        }
    } else {
        m_data = duality::defaultTransferFunctionData();
        m_initRequired = true;
    }
}

void TransferFunction::bindTexture() const {
    m_texture->bindWithUnit(0);
}

void TransferFunction::readData(const std::vector<uint8_t>& data) {
    std::string tfString(reinterpret_cast<const char*>(data.data()), data.size());
    std::stringstream stream(tfString);
    std::string line;
    size_t lineCount = 0;
    while (std::getline(stream, line) && lineCount < 256) {
        std::vector<float> tokens = mocca::splitString<float>(line, ' ');
        if (tokens.size() < 4) {
            throw Error("Incorrect number of tokens per line in transfer function file; must be 4", __FILE__, __LINE__);
        }
        for (size_t i = 0; i < 4; ++i) {
            m_data[lineCount][i] = static_cast<int8_t>(255 * tokens[i]);
        }
        lineCount++;
    }
    if (!stream.eof()) {
        throw Error("Incorrect number of lines in transfer function file; must be 256", __FILE__, __LINE__);
    }
}

void TransferFunction::initTexture() {
    if (!m_initRequired) {
        return;
    }
    
    // apply opacity correction
    TransferFunctionData correctedTf = m_data;
    float quality = 1.0f;
    for (int i = 0; i < 256; i++) {
        double alpha = correctedTf[i][3] / 255.0;
        alpha = 1.0 - std::pow(1.0 - alpha, 1.0 / quality);
        correctedTf[i][3] = static_cast<uint8_t>(255.0 * alpha);
    }
    m_texture = std::make_unique<GLTexture2D>(correctedTf.data(), GLTexture2D::TextureData::Color, 256, 1);
    m_initRequired = false;
}
