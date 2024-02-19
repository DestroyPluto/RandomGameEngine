#pragma once;
#include <cstdint>

namespace core{

    enum class HgError : uint8_t{
        eFailure = -1,
        eSuccess = 0,
        eNotImplemented = 1
    };

}