#pragma once

#define B_EVAL0(...) __VA_ARGS__
#define B_EVAL1(...) B_EVAL0(B_EVAL0(B_EVAL0(__VA_ARGS__)))
#define B_EVAL2(...) B_EVAL1(B_EVAL1(B_EVAL1(__VA_ARGS__)))
#define B_EVAL3(...) B_EVAL2(B_EVAL2(B_EVAL2(__VA_ARGS__)))
#define B_EVAL4(...) B_EVAL3(B_EVAL3(B_EVAL3(__VA_ARGS__)))
#define B_EVAL(...)  B_EVAL4(B_EVAL4(B_EVAL4(__VA_ARGS__)))

#define B_MAP_END(...)
#define B_MAP_OUT
#define B_MAP_COMMA ,

#define B_MAP_GET_END2() 0, B_MAP_END
#define B_MAP_GET_END1(...) B_MAP_GET_END2
#define B_MAP_GET_END(...) B_MAP_GET_END1
#define B_MAP_NEXT0(test, next, ...) next B_MAP_OUT
#define B_MAP_NEXT1(test, next) B_MAP_NEXT0(test, next, 0)
#define B_MAP_NEXT(test, next)  B_MAP_NEXT1(B_MAP_GET_END test, next)

#define B_MAP0(f, x, peek, ...) f(x) B_MAP_NEXT(peek, B_MAP1)(f, peek, __VA_ARGS__)
#define B_MAP1(f, x, peek, ...) f(x) B_MAP_NEXT(peek, B_MAP0)(f, peek, __VA_ARGS__)

#define B_MAP_LIST_NEXT1(test, next) B_MAP_NEXT0(test, B_MAP_COMMA next, 0)
#define B_MAP_LIST_NEXT(test, next)  B_MAP_LIST_NEXT1(B_MAP_GET_END test, next)

#define B_MAP_LIST0(f, x, peek, ...) f(x) B_MAP_LIST_NEXT(peek, B_MAP_LIST1)(f, peek, __VA_ARGS__)
#define B_MAP_LIST1(f, x, peek, ...) f(x) B_MAP_LIST_NEXT(peek, B_MAP_LIST0)(f, peek, __VA_ARGS__)

/**
 * Applies the function macro `f` to each of the remaining parameters.
 */
#define B_MAP(f, ...) B_EVAL(B_MAP1(f, __VA_ARGS__, ()()(), ()()(), ()()(), 0))

/**
 * Applies the function macro `f` to each of the remaining parameters and
 * inserts commas between the results.
 */
#define B_MAP_LIST(f, ...) B_EVAL(B_MAP_LIST1(f, __VA_ARGS__, ()()(), ()()(), ()()(), 0))