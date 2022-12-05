#include "asm/tools.h"

#include <fileioc.h>
#include <stdint.h>
#include <graphx.h>
#include <keypadc.h>

int main(void) {
    unsigned int offset = 1; // Stacks begin at a different offset

    char *stack = malloc(128 * 9);
    getLists(&offset, stack);
    getLists(&offset, stack + 128);
    getLists(&offset, stack + 256);
    getLists(&offset, stack + 384);
    getLists(&offset, stack + 512);
    getLists(&offset, stack + 640);
    getLists(&offset, stack + 768);
    getLists(&offset, stack + 896);
    getLists(&offset, stack + 1024);

    unsigned int moveCount;
    unsigned int moveStart;
    unsigned int moveEnd;

    offset = 0;
    getStart(&offset);

    uint8_t slot = ti_Open("Input", "r");
    unsigned int inputSize = ti_GetSize(slot);
    ti_Close(slot);

    gfx_Begin();
    while (offset < inputSize) {
        getMove(&offset, &moveCount, &moveStart, &moveEnd);
        for (uint8_t i = moveCount; i < 128; i++) {
            stack[moveEnd * 128 - 1 - i + moveCount] = stack[moveEnd * 128 - 1 - i]; // Shift right for more space
        }
        for (uint8_t i = 0; i < moveCount; i++) {
            stack[(moveEnd - 1) * 128 + moveCount - 1 - i] = stack[(moveStart - 1) * 128 + i];
            stack[(moveStart - 1) * 128 + i] = '\0';
        }
        for (uint8_t i = 0; i < 128 - moveCount; i++) {
            stack[(moveStart - 1) * 128 + i] = stack[(moveStart - 1) * 128 + i + moveCount]; 
        }
    }

    char *result = malloc(9);
    for (uint8_t i = 0; i < 9; i++) {
        result[i] = stack[i * 128];
    }

    gfx_PrintStringXY(result, 0, 0);
    while (!kb_AnyKey());
    gfx_End();

    free(stack);
    free(result);

    return 0;
}
