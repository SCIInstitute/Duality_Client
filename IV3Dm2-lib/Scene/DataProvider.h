#pragma once

#include "Scene/InputVariable.h"

#include <memory>
#include <vector>

class DataProvider {
public:
    virtual ~DataProvider() {}
    virtual std::shared_ptr<std::vector<uint8_t>> fetch() = 0;
    virtual std::vector<InputVariableFloat*> inputVariablesFloat() { return std::vector<InputVariableFloat*>(); }
    virtual std::vector<InputVariableEnum*> inputVariablesEnum() { return std::vector<InputVariableEnum*>(); }
};