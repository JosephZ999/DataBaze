#pragma once
#include <cstdio>

#ifdef _WIN32
#include <Windows.h>

#ifdef NDEBUG // ���� ��������� NDEBUG (Release �����)
#define LOG(...) do { } while (false) // ������ ������, ���� �� ����������
#else
#define LOG(...) do { \
    char msg[100]; \
    sprintf_s(msg, 100, __VA_ARGS__); \
    OutputDebugStringA(msg); \
} while (false)
#endif // NDEBUG

#else
#include <iostream>

#ifdef NDEBUG // ���� ��������� NDEBUG (Release �����)
#define LOG(...) do { } while (false) // ������ ������, ���� �� ����������
#else
#define LOG(...) do { \
    std::cout << __VA_ARGS__ << std::endl; \
} while (false)
#endif // NDEBUG

#endif // _WIN32