#include "gmock/gmock.h"

#include "Scene/DatasetProvider.h"

class DatasetProviderMock : public DatasetProvider {
public:
    MOCK_CONST_METHOD1(downloadDataset, std::unique_ptr<Dataset>(const std::string&));
};