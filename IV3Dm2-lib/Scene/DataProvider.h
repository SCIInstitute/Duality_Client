#pragma once

#include "Scene/DataProviderDispatcher.h"

class DataProvider {
public:
    virtual ~DataProvider() {}
    virtual void accept(DataProviderDispatcher& dispatcher) = 0;
};