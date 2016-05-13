#include "gmock/gmock.h"

#include "Scene/DataProvider.h"

class DataProviderMock : public DataProvider {
public:
    MOCK_METHOD1(accept, void(DataProviderDispatcher&));
};