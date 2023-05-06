
#include "battery/core/console.hpp"
#include "battery/core/string.hpp"
#include "battery/core/log.hpp"
#include "battery/core/environment.hpp"
#include "battery/core/extern/magic_enum.hpp"

#ifdef BATTERY_ARCH_WINDOWS
#include <conio.h>
#include <Windows.h>
#else
#include <termios.h>
#include <sys/ioctl.h>
#include <asm-generic/ioctls.h>
#endif

#include <cstdio>
#include <iostream>

#ifdef DELETE   // Stupid windows api
#undef DELETE
#endif

namespace b {

    std::u8string cin_getline(int buffer_size) {          // We must do this so complicated because std::cin does not have UTF-8 support
#ifdef BATTERY_ARCH_WINDOWS
        DWORD read = 0;
        std::wstring buffer;
        buffer.resize(buffer_size);

        ReadConsoleW(GetStdHandle(STD_INPUT_HANDLE),
                     buffer.data(),
                     buffer.size(),
                     &read,
                     nullptr);

        auto line = b::osstring_to_u8(buffer.substr(0, read));
#else
        std::string _line;
        std::getline(std::cin, line);           // TODO: buffer size is not used
        auto line = b::parse_u8_from_std_string(_line);
#endif
        line = b::replace(line, u8"\n", u8"");
        line = b::replace(line, u8"\r", u8"");
        return line;
    }

    namespace console {

        struct terminal::terminal_data {
#ifndef BATTERY_ARCH_WINDOWS
            struct termios previous_terminal_state;
#else
            CONSOLE_CURSOR_INFO previousCursorInfo;
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
#else
            HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
            CONSOLE_CURSOR_INFO cursorInfo;
            GetConsoleCursorInfo(out, &data->previousCursorInfo);
            GetConsoleCursorInfo(out, &cursorInfo);
            cursorInfo.bVisible = false;
            SetConsoleCursorInfo(out, &cursorInfo);
#endif
        }

        terminal::~terminal() {
#ifndef BATTERY_ARCH_WINDOWS
            tcsetattr(0, TCSANOW, &data->previous_terminal_state);
#else
            HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
            SetConsoleCursorInfo(out, &data->previousCursorInfo);
#endif
        }

        static keycode make_simple_key(int ch) {
            if (ch >= 'a' && ch <= 'z') {
                return (keycode) ch;
            }
            if (ch >= 'A' && ch <= 'Z') {
                return (keycode) (ch + ('a' - 'A'));
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
#ifdef BATTERY_ARCH_WINDOWS
                case 75:
                    return keycode::LEFT;
                case 77:
                    return keycode::RIGHT;
                case 72:
                    return keycode::UP;
                case 80:
                    return keycode::DOWN;
                case 83:
                    return keycode::DELETE;
                case 82:
                    return keycode::INSERT;
#else
                    case 68:
                        return keycode::LEFT;
                    case 67:
                        return keycode::RIGHT;
                    case 65:
                        return keycode::UP;
                    case 66:
                        return keycode::DOWN;
                    case 51:
                        return keycode::DELETE;
                    case 50:
                        return keycode::INSERT;
#endif
                default:
                    return keycode::NONE;
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

    } // namespace console

} // namespace b