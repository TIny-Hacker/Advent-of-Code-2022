#include "asm/tools.h"

#include <fileioc.h>
#include <stdint.h>
#include <graphx.h>
#include <keypadc.h>

int main(void) {
    unsigned int offset = 0;
    bool nextInstruction = true;

    char instruction = '\0';
    unsigned int cycle = 1;

    int total = 0;
    int registerX = 1;

    uint8_t slot = ti_Open("Input", "r");
    unsigned int inputSize = ti_GetSize(slot);
    ti_Close(slot);

    while (offset < inputSize) {
        if (nextInstruction == true) {
            instruction = getChar(offset);
            switch (instruction) {
                case 'n': // noop
                    nextInstruction = true;
                    offset += 5; // skip to next
                    break;
                case 'a':
                    nextInstruction = false;
                    break;
                default:
                    break;
            }
        } else { // Adding
            nextInstruction = true;
            registerX += getNumber(&offset);
        }
        cycle++;
        if (cycle == 20 || cycle == 60 || cycle == 100 || cycle == 140 || cycle == 180 || cycle == 220) {
            total += cycle * registerX;
        }
    }

    gfx_Begin();
    gfx_SetTextXY(0, 0);
    gfx_PrintInt(total, 1);
    while (!kb_AnyKey());
    gfx_End();

    return 0;
}
