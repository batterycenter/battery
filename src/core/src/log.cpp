
#include "battery/core/log.hpp"
#include <iostream>

#define RANG_IMPLEMENTATION                         // This file defines the implementation: By doing so, it separates
#include "battery/core/internal/extern/rang.hpp"    // <iostream>, the rang library and in turn the Windows API from the header
                                                    // Thus, this cannot be defined in the header, or we'd be
namespace b {                                       // polluting the global namespace with the Windows API and iostream

}
