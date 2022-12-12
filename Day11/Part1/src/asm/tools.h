#ifndef TOOLS_H
#define TOOLS_H

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

unsigned int monkeyCount(void);

char *nextMonkey(unsigned int *offset);

unsigned int getNumber(char *pointer);

#ifdef __cplusplus
}
#endif

#endif
