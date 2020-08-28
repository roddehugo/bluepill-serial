#ifndef common_h
#define common_h

#include <inttypes.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void halt(const char *msg);

void assertm(bool condition, const char *msg);

#define assert(c) assertm(c, #c)

#ifndef NDEBUG
# define vprintf(...) fprintf(stdout, __VA_ARGS__)
# define eprintf(...) fprintf(stderr, __VA_ARGS__)
#else
# define vprintf(x) ((void) 0)
# define eprintf(x) ((void) 0)
#endif

#endif
