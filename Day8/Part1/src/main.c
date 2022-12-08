#include <fileioc.h>
#include <stdint.h>
#include <graphx.h>
#include <keypadc.h>
#include <string.h>

static bool checkVisible(char *dataStart, unsigned int inputWidth, unsigned int inputHeight,
unsigned int x, unsigned int y) {
    char checkByte = dataStart[x + y * (inputWidth + 1)]; // We can just compare as characters

    bool visible = false;

    // Check up and down
    for (unsigned int i = 0; i < y; i++) {
        if (dataStart[x + i * (inputWidth + 1)] >= checkByte) {
            visible = false;
            break;
        } else {
            visible = true;
        }
    }

    if (visible == true) {
        return true;
    }

    for (unsigned int i = y + 1; i < inputHeight - 1; i++) {
        if (dataStart[x + i * (inputWidth + 1)] >= checkByte) {
            visible = false;
            break;
        } else {
            visible = true;
        }
    }

    if (visible == true) {
        return true;
    }

    // Check right and left
    for (unsigned int i = 0; i < x; i++) {
        if (dataStart[i + y * (inputWidth + 1)] >= checkByte) {
            visible = false;
            break;
        } else {
            visible = true;
        }
    }

    if (visible == true) {
        return true;
    }

    for (unsigned int i = x + 1; i < inputWidth; i++) {
        if (dataStart[i + y * (inputWidth + 1)] >= checkByte) {
            visible = false;
            break;
        } else {
            visible = true;
        }
    }

    if (visible == true) {
        return true;
    }

    return false; // Is not visible
}

int main(void) {
    uint8_t slot = ti_Open("Input", "r");
    unsigned int inputSize = ti_GetSize(slot);
    char *data = ti_GetDataPtr(slot);
    ti_Close(slot);

    unsigned int inputWidth = strlen(data);
    unsigned int inputHeight = 0;

    for (unsigned int i = 0; i < inputSize; i += inputWidth) {
        inputHeight++;
    }

    inputHeight--;

    unsigned int visibleTrees = inputWidth * 2 + inputHeight * 2 - 4; // Count edges, ignore corner overlaps
    gfx_Begin();


    uint8_t colors[10] = {38, 6, 37, 5, 36, 4, 35, 3, 34, 2};

    for (unsigned int y = 1; y < inputHeight - 1; y++) {
        for (unsigned int x = 1; x < inputWidth; x++) {
            gfx_SetColor(colors[(data[x + y * (inputWidth + 1)] - '0')]);
            gfx_SetPixel(x, y);
        }
    }

    gfx_SetColor(229);
    gfx_Rectangle_NoClip(0, 0, inputWidth, inputHeight); // The edge is always visible

    for (unsigned int y = 1; y < inputHeight - 1; y++) { // Ignore edges
        for (unsigned int x = 1; x < inputWidth; x++) {
            if (checkVisible(data, inputWidth, inputHeight, x, y)) {
                visibleTrees++;
                gfx_SetPixel(x, y); // Neat visual
            }
        }
    }

    gfx_SetTextXY(0, inputHeight + 3);
    gfx_PrintInt(visibleTrees, 0);
    while (!kb_AnyKey());
    gfx_End();

    return 0;
}


