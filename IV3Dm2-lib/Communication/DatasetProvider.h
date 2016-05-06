#pragma once

#include "Scene/Dataset.h"

#include <string>
#include <memory>

class DatasetProvider {
public:
    virtual std::unique_ptr<Dataset> downloadDataset(const std::string& path) const = 0;
};