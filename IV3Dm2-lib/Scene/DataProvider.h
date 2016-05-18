#pragma once

#include "Scene/DataProviderDispatcher.h"

#include <memory>
#include <vector>

class DataProvider {
public:
    virtual ~DataProvider() {}
    virtual void accept(DataProviderDispatcher& dispatcher) = 0;
    virtual std::shared_ptr<std::vector<uint8_t>> fetch() = 0;
};