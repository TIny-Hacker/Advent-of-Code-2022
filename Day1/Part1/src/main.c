#include "asm/getTotal.h"

#include <fileioc.h>
#include <stdint.h>
#include <graphx.h>
#include <keypadc.h>

int main(void) {
    unsigned int maxNumber = 0;
    unsigned int currentNumber = 0;
    unsigned int offset = 0;

    uint8_t slot = ti_Open("Input", "r");
    unsigned int inputSize = ti_GetSize(slot);
    ti_Close(slot);

    while (offset < inputSize) {
        currentNumber = getTotal(&offset);
        if (currentNumber > maxNumber) {
            maxNumber = currentNumber;
        }
    }

    gfx_Begin();
    gfx_SetTextXY(0, 0);
    gfx_PrintInt(maxNumber, 0);
    while (!kb_AnyKey());
    gfx_End();
}