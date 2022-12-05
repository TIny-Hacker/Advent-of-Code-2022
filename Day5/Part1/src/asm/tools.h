#ifndef TOOLS_H
#define TOOLS_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void getLists(unsigned int *offset, char *stack);

void getMove(unsigned int *offset, unsigned int *count, unsigned int *start, unsigned int *end);

void getStart(unsigned int *offset);

#ifdef __cplusplus
}
#endif

#endif
