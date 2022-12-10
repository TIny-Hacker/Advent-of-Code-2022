#include "asm/tools.h"

#include <fileioc.h>
#include <stdint.h>
#include <graphx.h>
#include <keypadc.h>

#define MAP_WIDTH   370
#define MAP_HEIGHT  370

#define START_X     MAP_WIDTH / 2
#define START_Y     MAP_HEIGHT / 2

int main(void) {
    unsigned int xValues[10] = {START_X, START_X, START_X, START_X, START_X, START_X, START_X, START_X, START_X, START_X};
    unsigned int yValues[10] = {START_Y, START_Y, START_Y, START_Y, START_Y, START_Y, START_Y, START_Y, START_Y, START_Y};

    unsigned int offset = 0;

    uint8_t slot = ti_Open("Input", "r");
    unsigned int inputSize = ti_GetSize(slot);
    ti_Close(slot);

    char instruction;
    unsigned int moveAmount;

    gfx_Begin();

    asm("call $0000374"); // Clear VRAM

    unsigned int tailCovered = 0;

    while (offset < inputSize) {
        tailCovered += putTail(xValues[0], yValues[0]);
        instruction = getChar(offset);
        moveAmount = getNumber(&offset); // Get number and update offset

        for (unsigned int i = 0; i < moveAmount; i++) {
            switch (instruction) {
                case 'U':
                    yValues[9]--;
                    break;
                case 'D':
                    yValues[9]++;
                    break;
                case 'L':
                    xValues[9]--;
                    break;
                case 'R':
                    xValues[9]++;
                    break;
                default:
                    break;
            }

            for (unsigned int j = 9; j; j--) {
                if (yValues[j] == yValues[j - 1] - 2) { // Above
                    yValues[j - 1]--;
                    if (xValues[j] > xValues[j - 1]) {
                        xValues[j - 1]++;
                    } else if (xValues[j] < xValues[j - 1]) {
                        xValues[j - 1]--;
                    }
                } else if (yValues[j] == yValues[j - 1] + 2) { // Below
                    yValues[j - 1]++;
                    if (xValues[j] > xValues[j - 1]) {
                        xValues[j - 1]++;
                    }  else if (xValues[j] < xValues[j - 1]) {
                        xValues[j - 1]--;
                    }
                } else if (xValues[j] == xValues[j - 1] - 2) { // Left
                    xValues[j - 1]--;
                    if (yValues[j] > yValues[j - 1]) {
                        yValues[j - 1]++;
                    } else if (yValues[j] < yValues[j - 1]) {
                        yValues[j - 1]--;
                    }
                } else if (xValues[j] == xValues[j - 1] + 2) { // Right
                    xValues[j - 1]++;
                    if (yValues[j] > yValues[j - 1]) {
                        yValues[j - 1]++;
                    } else if (yValues[j] < yValues[j - 1]) {
                        yValues[j - 1]--;
                    }
                }
            }

            tailCovered += putTail(xValues[0], yValues[0]);
        }
    }

    gfx_SetTextXY(0, 0);
    gfx_PrintInt(tailCovered, 1);
    while (!kb_AnyKey());
    gfx_End();

    return 0;
}
