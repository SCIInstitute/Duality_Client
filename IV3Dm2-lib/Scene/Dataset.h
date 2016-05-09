#pragma once

#include <vector>
#include <memory>

class AbstractRenderer;

class Dataset {
public:
    virtual ~Dataset() {}

    static std::unique_ptr<Dataset> create(const std::vector<uint8_t>& binaryData);
    
    virtual void render(AbstractRenderer& renderer) const = 0;
};