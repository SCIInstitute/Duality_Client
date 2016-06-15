#pragma once

#include "src/duality/DataProvider.h"
#include "src/duality/GLTexture2D.h"

#include <array>
#include <memory>

using TransferFunctionData = std::array<std::array<uint8_t, 4>, 256>;

class TransferFunction {
public:
    TransferFunction(std::unique_ptr<DataProvider> provider);

    void fetch();
    void bindTexture() const;

    std::vector<FloatVariableInfo> floatVariableInfos() const;
    std::vector<EnumVariableInfo> enumVariableInfos() const;
    bool hasFloatVariable(const std::string& variableName) const;
    bool hasEnumVariable(const std::string& variableName) const;
    void setVariable(const std::string& variableName, float value);
    void setVariable(const std::string& variableName, const std::string& value);

private:
    void readData(const std::vector<uint8_t>& data);
    void initTexture();

private:
    std::unique_ptr<DataProvider> m_provider;
    TransferFunctionData m_data;
    std::unique_ptr<GLTexture2D> m_texture;
};


namespace duality {
    TransferFunctionData defaultTransferFunctionData();
}