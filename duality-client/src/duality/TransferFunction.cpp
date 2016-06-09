#include "src/duality/TransferFunction.h"

TransferFunction duality::defaultTransferFunction() {
    TransferFunction tf;
    for (uint8_t i = 0; i <= static_cast<uint8_t>(255); ++i) {
        tf[i] = {i, i, i, (i < 50) ? static_cast<uint8_t>(0) : i};
    }
    return tf;
}
