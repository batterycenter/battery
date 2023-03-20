#pragma once

#include "battery/core/environment.hpp"

namespace b::console {

    enum class key {
        NONE = 0,
        BACKSPACE = 8,
        TAB = 9,
        LINEFEED = 10,
        ENTER = 13,
        CANCEL = 24,
        ESCAPE = 27,
        SPACE = ' ',
        PLUS = '+',
        MINUS = '-',
        DOT = '.',
        COLON = ':',
        COMMA = ',',
        SEMICOLON = ';',
        ASTERISK = '*',
        PERCENT = '%',
        DIVIDE = '/',
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
        LEFT,
        RIGHT,
        DOWN,
        UP,
        DELETE
    };

    struct keycode {
        key key;

    };

    key get_control_key();

    int raw_getch();

}
