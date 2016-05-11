#pragma once

#include <memory>
#include <vector>

class DataProvider {
public:
    virtual std::shared_ptr<std::vector<uint8_t>> fetch() = 0;
};