#pragma once

#include <vector>
#include <memory>

class Dataset {
public:
    virtual ~Dataset() {}

    static std::unique_ptr<Dataset> create(const std::vector<uint8_t>& binaryData);
    
    //virtual void render(Renderer& renderer) const = 0;
    virtual std::unique_ptr<Dataset> clone() = 0;
};