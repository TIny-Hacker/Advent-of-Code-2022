#include "asm/getLine.h"

#include <fileioc.h>
#include <stdint.h>
#include <graphx.h>
#include <keypadc.h>

int main(void) {
    unsigned int total = 0;
    unsigned int offset = 0;
    char *string;

    uint8_t slot = ti_Open("Input", "r");
    unsigned int inputSize = ti_GetSize(slot);
    ti_Close(slot);

    while (offset < inputSize) {
        string = getLine(&offset);

        if (string[0] == 'A') {
            switch (string[2]) {
                case 'X':
                    total += 3;
                    break;
                case 'Y':
                    total += 4;
                    break;
                case 'Z':
                    total += 8;
                    break;
                default:
                    break;
            }
        } else if (string[0] == 'B') {
            switch (string[2]) {
                case 'X':
                    total += 1;
                    break;
                case 'Y':
                    total += 5;
                    break;
                case 'Z':
                    total += 9;
                    break;
                default:
                    break;
            }
        } else if (string[0] == 'C') {
            switch (string[2]) {
                case 'X':
                    total += 2;
                    break;
                case 'Y':
                    total += 6;
                    break;
                case 'Z':
                    total += 7;
                    break;
                default:
                    break;
            }
        }
    }

    gfx_Begin();
    gfx_SetTextXY(0, 0);
    gfx_PrintInt(total, 0);
    while (!kb_AnyKey());
    gfx_End();
}
