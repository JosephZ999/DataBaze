#pragma once
#include <cstdio>

#ifdef _WIN32
#include <Windows.h>

#ifdef NDEBUG // Если определен NDEBUG (Release режим)
#define LOG(...) do { } while (false) // Пустой макрос, логи не печатаются
#else
#define LOG(...) do { \
    char msg[100]; \
    sprintf_s(msg, 100, __VA_ARGS__); \
    OutputDebugStringA(msg); \
} while (false)
#endif // NDEBUG

#else
#include <iostream>

#ifdef NDEBUG // Если определен NDEBUG (Release режим)
#define LOG(...) do { } while (false) // Пустой макрос, логи не печатаются
#else
#define LOG(...) do { \
    std::cout << __VA_ARGS__ << std::endl; \
} while (false)
#endif // NDEBUG

#endif // _WIN32