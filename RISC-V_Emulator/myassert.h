#pragma once

#include <stdio.h>
#include <stdlib.h>

void my_assert(const char* exp, const char* func, const char* file, unsigned int line)
{
#ifdef _DEBUG
    __debugbreak();
#endif
    fprintf(stderr, "ERROR: %s %s\n%s %d\n", func, exp, file, line);
    scanf_s("Press enter to exit.");
    _Exit(EXIT_FAILURE);
}

#define myassert(expression) if (expression) { my_assert(#expression, __FUNCTION__, __FILE__, __LINE__); }