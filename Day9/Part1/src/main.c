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
    unsigned int headX = START_X, tailX = START_X;
    unsigned int headY = START_Y, tailY = START_Y;

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
        tailCovered += putTail(tailX, tailY);
        instruction = getChar(offset);
        moveAmount = getNumber(&offset); // Get number and update offset
        switch (instruction) {
            case 'U':
                for (unsigned int i = 0; i < moveAmount; i++) {
                    headY--;
                    if (tailY - headY > 1) { // Mind the gap
                        tailCovered += putTail(tailX, tailY);
                        tailY--;
                        if (headX < tailX) { // Move diagonally
                            tailX--;
                        } else if (headX > tailX) {
                            tailX++;
                        }
                    }
                }
                break;
            case 'D':
                for (unsigned int i = 0; i < moveAmount; i++) {
                    headY++;
                    if (headY - tailY > 1) {
                        tailCovered += putTail(tailX, tailY);
                        tailY++;
                        if (headX < tailX) { // Move diagonally
                            tailX--;
                        } else if (headX > tailX) {
                            tailX++;
                        }
                    }
                }
                break;
            case 'L':
                for (unsigned int i = 0; i < moveAmount; i++) {
                    headX--;
                    if (tailX - headX > 1) {
                        tailCovered += putTail(tailX, tailY);
                        tailX--;
                        if (headY < tailY) { // Move diagonally
                            tailY--;
                        } else if (headY > tailY) {
                            tailY++;
                        }
                    }
                }
                break;
            case 'R':
                for (unsigned int i = 0; i < moveAmount; i++) {
                    headX++;
                    if (headX - tailX > 1) {
                        tailCovered += putTail(tailX, tailY);
                        tailX++;
                        if (headY < tailY) { // Move diagonally
                            tailY--;
                        } else if (headY > tailY) {
                            tailY++;
                        }
                    }
                }
                break;
            default:
                break;
        }
    }

    gfx_SetTextXY(0, 0);
    gfx_PrintInt(tailCovered, 1);
    while (!kb_AnyKey());
    gfx_End();

    return 0;
}
