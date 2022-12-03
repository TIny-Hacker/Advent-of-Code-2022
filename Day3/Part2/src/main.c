#include "asm/getString.h"

#include <fileioc.h>
#include <stdint.h>
#include <graphx.h>
#include <keypadc.h>

static uint8_t checkSimilar(const char *string1, const char *string2, const char *string3, unsigned int *length) {
    for (uint8_t i = 0; i < length[0]; i++) {
        for (uint8_t j = 0; j < length[1]; j++) {
            if (string1[i] == string2[j]) {
                for (uint8_t k = 0; k < length[2]; k++) { // Very nested loops
                    if (string1[i] == string3[k]) {
                        if (string1[i] >= 'a') {
                            return string1[i] - 96;
                        } else {
                            return string1[i] - 38;
                        }
                    }
                }
            }
        }
    }

    return 0;
}

int main(void) {
    unsigned int total = 0;
    unsigned int offset = 0;
    unsigned int length[3] = {0, 0, 0};
    char *string1;
    char *string2;
    char *string3;

    gfx_Begin();
    uint8_t slot = ti_Open("Input", "r");
    unsigned int inputSize = ti_GetSize(slot);
    ti_Close(slot);

    while (offset < inputSize) {
        string1 = getString(&offset, &length[0]);
        string2 = getString(&offset, &length[1]);
        string3 = getString(&offset, &length[2]);
        total += checkSimilar(string1, string2, string3, length);
    }

    gfx_SetTextXY(0, 0);
    gfx_PrintInt(total, 0);
    while (!kb_AnyKey());
    gfx_End();
}
