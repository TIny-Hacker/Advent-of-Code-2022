#include "asm/getNumbers.h"

#include <fileioc.h>
#include <stdint.h>
#include <graphx.h>
#include <keypadc.h>

int main(void) {
    unsigned int low1, high1, low2, high2;  // Numbers to check
    unsigned int offset = 0;
    unsigned int total = 0;

    uint8_t slot = ti_Open("Input", "r");
    unsigned int inputSize = ti_GetSize(slot);
    ti_Close(slot);

    while (offset < inputSize) {
        getNumbers(&offset, &low1, &high1, &low2, &high2);
        if ((low2 <= high1 && low2 >= low1) || (high2 <= high1 && high2 >= low1)) {
            total++;
        } else if ((low1 <= high2 && low1 >= low2) || (high1 <= high2 && high1 >= low2)) {
            total++;
        }
    }

    gfx_Begin();
    gfx_SetTextXY(0, 0);
    gfx_PrintInt(total, 0);
    while (!kb_AnyKey());
    gfx_End();
}