#include "asm/getTotal.h"

#include <fileioc.h>
#include <stdint.h>
#include <graphx.h>
#include <keypadc.h>

int main(void) {
    unsigned int maxNumbers[3] = {0, 0, 0};
    unsigned int currentNumber = 0;
    unsigned int offset = 0;
    uint8_t slot = ti_Open("Input", "r");
    unsigned int inputSize = ti_GetSize(slot);
    ti_Close(slot);

    while (offset < inputSize) {
        currentNumber = getTotal(&offset);

        // Not a very elegant way to do this but whatever
        if (currentNumber > maxNumbers[2]) {
            maxNumbers[0] = maxNumbers[1];
            maxNumbers[1] = maxNumbers[2];
            maxNumbers[2] = currentNumber;
        } else if (currentNumber > maxNumbers[1]) {
            maxNumbers[0] = maxNumbers[1];
            maxNumbers[1] = currentNumber;
        } else if (currentNumber > maxNumbers[0]) {
            maxNumbers[0] = currentNumber;
        }
    }

    gfx_Begin();
    gfx_SetTextXY(0, 0);
    gfx_PrintInt(maxNumbers[0] + maxNumbers[1] + maxNumbers[2], 0);
    while (!kb_AnyKey());
    gfx_End();
}