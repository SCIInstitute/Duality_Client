#pragma once

#include "src/duality/DataProvider.h"
#include "src/duality/GLTexture2D.h"

#include <array>
#include <memory>

using TransferFunctionData = std::array<std::array<uint8_t, 4>, 256>;

class TransferFunction {
public:
    TransferFunction(std::unique_ptr<DataProvider> provider);

    void update();
    void initTexture();
    void bindTexture() const;

private:
    void readData(const std::vector<uint8_t>& data);

private:
    std::unique_ptr<DataProvider> m_provider;
    bool m_initRequired;
    TransferFunctionData m_data;
    std::unique_ptr<GLTexture2D> m_texture;
};


namespace duality {
    TransferFunctionData defaultTransferFunctionData();
}