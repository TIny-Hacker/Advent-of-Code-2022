#include "asm/tools.h"

#include <fileioc.h>
#include <stdint.h>
#include <graphx.h>
#include <keypadc.h>

int main(void) {
    unsigned int offset = 0;
    bool nextInstruction = true;

    char instruction = '\0';
    int cycle = 1;

    int registerX = 1;

    unsigned int drawX = 1;
    unsigned int drawY = 0;

    uint8_t slot = ti_Open("Input", "r");
    unsigned int inputSize = ti_GetSize(slot);
    ti_Close(slot);

    gfx_Begin();
    gfx_FillRectangle(0, 0, 2, 2); // Setting pixels is too small

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

        if (cycle % 40 >= registerX - 1 && cycle % 40 <= registerX + 1) {
            gfx_FillRectangle(drawX * 2, drawY * 2, 2, 2);
        }

        cycle++;
        drawX++;
        if (cycle == 40 || cycle == 80 || cycle == 120 || cycle == 160 || cycle == 200 || cycle == 240) {
            drawY++;
            drawX = 0;
        }
    }

    while (!kb_AnyKey());
    gfx_End();

    return 0;
}
