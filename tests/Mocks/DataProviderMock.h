#include "gmock/gmock.h"

#include "duality/DataProvider.h"

class DataProviderMock : public DataProvider {
public:
    MOCK_METHOD0(fetch, std::shared_ptr<std::vector<uint8_t>>());
};