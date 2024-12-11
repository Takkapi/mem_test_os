#ifndef SNPRINTF_H
#define SNPRINF_H

#include <stddef.h>
#include <stdarg.h>

int snprintf(char* buffer, size_t size, const char* format, ...);

#endif