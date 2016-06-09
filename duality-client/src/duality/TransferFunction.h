#pragma once

#include <array>

using TransferFunction = std::array<std::array<uint8_t, 4>, 256>;

namespace duality {
    TransferFunction defaultTransferFunction();
}