
#include "battery/core/console.hpp"
#include "battery/core/string.hpp"
#include "battery/core/log.hpp"
#include "battery/core/process.hpp"
#include "battery/core/extern/magic_enum.hpp"

#ifdef B_OS_WINDOWS
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

    b::string cin_getline(size_t buffer_size) {          // We must do this so complicated because std::cin does not have UTF-8 support
#ifdef B_OS_WINDOWS
        DWORD read = 0;
        std::wstring buffer;
        buffer.resize(buffer_size);

        ReadConsoleW(GetStdHandle(STD_INPUT_HANDLE),
                     buffer.data(),
                     buffer.size(),
                     &read,
                     nullptr);

        auto line = b::string::from_native(buffer.substr(0, read));
#else
        b::string line;
        std::getline(std::cin, line);
        if (line.size() > buffer_size) {    // Limit to buffer size for consistent behavior across platforms
            line = line.substr(0, buffer_size);
        }
#endif
        line.replace("\n"_b, ""_b);
        line.replace("\r"_b, ""_b);
        return line;
    }

    bool open_url_in_default_browser(const b::string& url) {
#ifdef B_OS_WINDOWS
        auto process = b::execute("start "_b + url);
        return process.exit_code == 0;
#else
        auto process = b::execute("xdg-open "_b + url);
        return process.exit_code == 0;
#endif
    }

    namespace console {

        struct terminal::terminal_data {
#ifndef B_OS_WINDOWS
            struct termios previous_terminal_state;
#else
            CONSOLE_CURSOR_INFO previousCursorInfo;
#endif
        };

        terminal::terminal() : data(std::make_unique<terminal::terminal_data>()) {
#ifndef B_OS_WINDOWS
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
#ifndef B_OS_WINDOWS
            tcsetattr(0, TCSANOW, &data->previous_terminal_state);
#else
            HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
            SetConsoleCursorInfo(out, &data->previousCursorInfo);
#endif
        }

        static keycode make_simple_key(int ch) {
            if (ch >= 'a' && ch <= 'z') {
                return static_cast<keycode>(ch);
            }
            if (ch >= 'A' && ch <= 'Z') {
                return static_cast<keycode>(ch + ('a' - 'A'));
            }
            switch (ch) {
                case 8: case 127:   return keycode::BACKSPACE;
                case 9:             return keycode::TAB;
                case 27:            return keycode::ESCAPE;
                case ' ':           return keycode::SPACE;
                case '+':           return keycode::PLUS;
                case '-':           return keycode::MINUS;
                case '.':           return keycode::DOT;
                case ':':           return keycode::COLON;
                case ',':           return keycode::COMMA;
                case ';':           return keycode::SEMICOLON;
                case '*':           return keycode::ASTERISK;
                case '%':           return keycode::PERCENT;
                case '/':           return keycode::DIVIDE;
                case 10: case 13:   return keycode::ENTER;
                default:
                    return keycode::NONE;
            }
        }

        static keycode make_special_key(int ch) {
            switch (ch) {
#ifdef B_OS_WINDOWS
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
#ifdef B_OS_WINDOWS
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
