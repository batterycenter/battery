
#include "battery/core/console.hpp"
#include "battery/core/environment.hpp"
#include "battery/core/extern/magic_enum.hpp"

#ifdef BATTERY_ARCH_WINDOWS
#include <conio.h>
#else
#include <curses.h>
#endif

#include <cstdio>

namespace b::console {

    static key make_simple_key(int ch) {
        if (auto k = magic_enum::enum_cast<key>(ch); k) {
            return k.value();
        }
        return key::NONE;
    }

    static key make_composed_key(int first, int second) {
        if (second == 75) {
            return key::LEFT;
        }
        else if (second == 77) {
            return key::RIGHT;
        }
        else if (second == 72) {
            return key::UP;
        }
        else if (second == 80) {
            return key::DOWN;
        }
        else if (second == 83) {
            return key::DELETE;
        }
        return key::NONE;
    }

    key get_control_key() {
#ifdef BATTERY_ARCH_WINDOWS
        int ch = _getch();
        if (ch != 0 && ch != 224) {     // 0 or 224 mean special key code
            return make_simple_key(ch);
        }
        return make_composed_key(ch, _getch());
#else
#endif
    }

    int raw_getch() {
#ifdef BATTERY_ARCH_WINDOWS
        return _getch();
#else
#endif
    }

}
