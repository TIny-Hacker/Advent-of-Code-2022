#include "asm/getString.h"

#include <fileioc.h>
#include <stdint.h>
#include <graphx.h>
#include <keypadc.h>

static uint8_t checkSimilar(const char *string, unsigned int length) {
    for (uint8_t i = 0; i < length / 2; i++) {
        for (uint8_t j = length / 2; j < length; j++) {
            if (string[i] == string[j]) {
                if (string[i] >= 'a') {
                    return string[i] - 96;
                } else {
                    return string[i] - 38;
                }
            }
        }
    }

    return 0;
}

int main(void) {
    unsigned int total = 0;
    unsigned int offset = 0;
    unsigned int length = 0;
    char *string;

    uint8_t slot = ti_Open("Input", "r");
    unsigned int inputSize = ti_GetSize(slot);
    ti_Close(slot);

    while (offset < inputSize) {
        string = getString(&offset, &length);
        total += checkSimilar(string, length);
    }

    gfx_Begin();
    gfx_SetTextXY(0, 0);
    gfx_PrintInt(total, 0);
    while (!kb_AnyKey());
    gfx_End();
}
