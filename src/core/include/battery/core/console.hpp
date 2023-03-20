#pragma once

#include <memory>
#include "battery/core/environment.hpp"

namespace b::console {

    enum class keycode : int64_t {
        NONE = 0,
        A = 'a',
        B = 'b',
        C = 'c',
        D = 'd',
        E = 'e',
        F = 'f',
        G = 'g',
        H = 'h',
        I = 'i',
        J = 'j',
        K = 'k',
        L = 'l',
        M = 'm',
        N = 'n',
        O = 'o',
        P = 'p',
        Q = 'q',
        R = 'r',
        S = 's',
        T = 't',
        U = 'u',
        V = 'v',
        W = 'w',
        X = 'x',
        Y = 'y',
        Z = 'z',
        BACKSPACE,
        TAB,
        ESCAPE,
        SPACE,
        PLUS,
        MINUS,
        DOT,
        COLON,
        COMMA,
        SEMICOLON,
        ASTERISK,
        PERCENT,
        DIVIDE,
        LEFT,
        RIGHT,
        DOWN,
        UP,
        INSERT,
        DELETE,
        ENTER
    };

    class terminal {
    public:
        struct terminal_data;

        terminal();
        ~terminal();

        keycode get_control_key();

    private:
        std::unique_ptr<terminal_data> data;
    };

}
