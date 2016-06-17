#pragma once

#include "src/duality/NodeDispatcher.h"
#include "src/duality/View.h"
#include "src/duality/DataProvider.h"

class Dataset {
public:
    Dataset(std::unique_ptr<DataProvider> provider);
    virtual ~Dataset() {}
    
    void fetch();

private:
    virtual void readData(const std::vector<uint8_t>& data) = 0;

private:
    std::unique_ptr<DataProvider> m_provider;
};