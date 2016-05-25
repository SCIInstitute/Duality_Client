#include "gmock/gmock.h"

#include "duality/Dataset.h"

class DatasetMock : public Dataset {
public:
    MOCK_METHOD1(accept, void(DatasetDispatcher&));
    MOCK_METHOD1(read, void(std::shared_ptr<std::vector<uint8_t>>));
    MOCK_METHOD1(applyTransform, void(const IVDA::Mat4f&));
};