
#include "battery/core/console.hpp"
#include "battery/core/environment.hpp"
#include "battery/core/extern/magic_enum.hpp"

#ifdef BATTERY_ARCH_WINDOWS
#include <conio.h>
#else
#include <termios.h>
#include <sys/ioctl.h>
#include <asm-generic/ioctls.h>
#endif

#include <cstdio>

//#include <ncurses.h>

namespace b::console {

    struct terminal::terminal_data {
#ifndef BATTERY_ARCH_WINDOWS
        struct termios previous_terminal_state;
#endif
    };

    terminal::terminal() : data(std::make_unique<terminal::terminal_data>()) {
#ifndef BATTERY_ARCH_WINDOWS
        static struct termios newtio;
        tcgetattr(0, &data->previous_terminal_state);
        newtio = data->previous_terminal_state;
        newtio.c_lflag &= ~ICANON;
        newtio.c_lflag &= ~ECHO;
        tcsetattr(0, TCSANOW, &newtio);
        fflush(stdout);
#endif
    }

    terminal::~terminal() {
#ifndef BATTERY_ARCH_WINDOWS
        tcsetattr(0, TCSANOW, &data->previous_terminal_state);
#endif
    }

    static keycode make_simple_key(int ch) {
        if (ch >= 'a' && ch <= 'z') {
            return (keycode)ch;
        }
        if (ch >= 'A' && ch <= 'Z') {
            return (keycode)(ch + ('a' - 'A'));
        }
        if (ch == 8) return keycode::BACKSPACE;
        if (ch == 127) return keycode::BACKSPACE;
        if (ch == 9) return keycode::TAB;
        if (ch == 27) return keycode::ESCAPE;
        if (ch == ' ') return keycode::SPACE;
        if (ch == '+') return keycode::PLUS;
        if (ch == '-') return keycode::MINUS;
        if (ch == '.') return keycode::DOT;
        if (ch == ':') return keycode::COLON;
        if (ch == ',') return keycode::COMMA;
        if (ch == ';') return keycode::SEMICOLON;
        if (ch == '*') return keycode::ASTERISK;
        if (ch == '%') return keycode::PERCENT;
        if (ch == '/') return keycode::DIVIDE;
        else if (ch == 10 || ch == 13) {
            return keycode::ENTER;
        }
        return keycode::NONE;
    }

    static keycode make_special_key(int ch) {
        switch (ch) {
            using enum keycode;
#ifdef BATTERY_ARCH_WINDOWS
            case 75:
                return LEFT;
            case 77:
                return RIGHT;
            case 72:
                return UP;
            case 80:
                return DOWN;
            case 83:
                return DELETE;
            case 82:
                return INSERT;
#else
            case 68:
                return LEFT;
            case 67:
                return RIGHT;
            case 65:
                return UP;
            case 66:
                return DOWN;
            case 51:
                return DELETE;
            case 50:
                return INSERT;
#endif
            default:
                return NONE;
        }
    }

    keycode terminal::get_control_key() {       // https://en.wikipedia.org/wiki/ANSI_escape_code
#ifdef BATTERY_ARCH_WINDOWS
        int ch = _getch();
        if (ch != 0 && ch != 224) {     // 0 or 224 mean special key code
            return make_simple_key(ch);
        }
        return make_special_key(_getch());
#else
        char s[3];
        ssize_t n = read(STDIN_FILENO, &s, sizeof(s));
        if (n == 3) {            // 3 characters for a special key
            if (s[0] == 27 && s[1] == '[') {          // 27 = ESC
                //printf("special: %d %d %d\n", s[0], s[1], s[2]);
                return make_special_key(s[2]);
            }
            return keycode::NONE;
        }
        else if (n == 2) {
            //printf("two: %d %d\n", s[0], s[1]);
            return keycode::NONE;
        }
        else if (n == 1) {
            //printf("simple: %d\n", s[0]);
            return make_simple_key(s[0]);
        }
        return keycode::NONE;
#endif
    }

}
