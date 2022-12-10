#ifndef TOOLS_H
#define TOOLS_H

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

unsigned int getNumber(unsigned int *offset);

char getChar(unsigned int offset);

bool putTail(unsigned int x, unsigned int y);

#ifdef __cplusplus
}
#endif

#endif
