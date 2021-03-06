#pragma once

#include "jsoncpp/json.h"

#include <memory>
#include <vector>

class DataProvider {
public:
    virtual ~DataProvider() {}
    virtual std::shared_ptr<std::vector<uint8_t>> fetch() = 0;
    virtual void notify() = 0;
};