#include "src/duality/TransferFunction.h"

TransferFunction duality::defaultTransferFunction() {
    TransferFunction tf;
    for (int i = 0; i <256; ++i) {
        uint8_t rgb = static_cast<uint8_t>(i);
        uint8_t alpha = (i < 50) ? static_cast<uint8_t>(0) : static_cast<uint8_t>(i);
        tf[i] = {rgb, rgb, rgb, alpha};
    }
    return tf;
}
