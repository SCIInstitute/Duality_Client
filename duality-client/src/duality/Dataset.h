#pragma once

#include "src/duality/NodeDispatcher.h"
#include "src/duality/View.h"
#include "src/duality/DataProvider.h"

class Dataset {
public:
    Dataset(std::unique_ptr<DataProvider> provider);
    
    void fetch();
    std::vector<FloatVariableInfo> floatVariableInfos() const;
    std::vector<EnumVariableInfo> enumVariableInfos() const;
    bool hasFloatVariable(const std::string& variableName) const;
    bool hasEnumVariable(const std::string& variableName) const;
    void setVariable(const std::string& variableName, float value);
    void setVariable(const std::string& variableName, const std::string& value);

private:
    virtual void readData(const std::vector<uint8_t>& data) = 0;

private:
    std::unique_ptr<DataProvider> m_provider;
};