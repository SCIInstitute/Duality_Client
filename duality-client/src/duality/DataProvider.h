#pragma once

#include "duality/VariableInfo.h"

#include "src/duality/InputVariable.h"

#include <memory>
#include <vector>

class DataProvider {
public:
    virtual ~DataProvider() {}
    virtual std::shared_ptr<std::vector<uint8_t>> fetch() = 0;
    virtual std::vector<FloatVariableInfo> floatVariableInfos() const { return {}; }
    virtual std::vector<EnumVariableInfo> enumVariableInfos() const { return {}; }
    virtual void setVariable(const std::string& variable, float value) {}
    virtual void setVariable(const std::string& variable, const std::string& value) {}
};