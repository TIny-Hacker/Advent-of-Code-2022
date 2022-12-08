#include <fileioc.h>
#include <stdint.h>
#include <graphx.h>
#include <keypadc.h>
#include <string.h>

static unsigned int getScore(char *dataStart, unsigned int inputWidth, unsigned int inputHeight,
unsigned int x, unsigned int y) {
    char checkByte = dataStart[x + y * (inputWidth + 1)]; // We can just compare as characters

    unsigned int topScore = 0, bottomScore = 0, leftScore = 0, rightScore = 0;

    // Check up and down
    if (y) {
        for (int i = y - 1; i >= 0; i--) {
            if (dataStart[x + i * (inputWidth + 1)] >= checkByte) {
                topScore++;
                break;
            } else {
                topScore++;
            }
        }
    }

    if (y < inputHeight) {
        for (unsigned int i = y + 1; i < inputHeight; i++) {
            if (dataStart[x + i * (inputWidth + 1)] >= checkByte) {
                bottomScore++;
                break;
            } else {
                bottomScore++;
            }
        }
    }

    // Check right and left
    if (x) {
        for (int i = x - 1; i >= 0; i--) {
            if (dataStart[i + y * (inputWidth + 1)] >= checkByte) {
                leftScore++;
                break;
            } else {
                leftScore++;
            }
        }
    }

    if (x < inputWidth + 1) {
        for (unsigned int i = x + 1; i < inputWidth; i++) {
            if (dataStart[i + y * (inputWidth + 1)] >= checkByte) {
                rightScore++;
                break;
            } else {
                rightScore++;
            }
        }
    }

    return topScore * bottomScore * leftScore * rightScore;
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

    unsigned int mostScenic = 0;
    unsigned int currentTreeScore = 0;

    for (unsigned int y = 0; y < inputHeight; y++) {
        for (unsigned int x = 0; x < inputWidth + 1; x++) {
            currentTreeScore = getScore(data, inputWidth, inputHeight, x, y);
            if (currentTreeScore > mostScenic) {
                mostScenic = currentTreeScore;
            }
        }
    }

    gfx_Begin();
    gfx_SetTextXY(0, 0);
    gfx_PrintInt(mostScenic, 0);
    while (!kb_AnyKey());
    gfx_End();

    return 0;
}


