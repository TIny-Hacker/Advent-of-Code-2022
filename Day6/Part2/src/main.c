#include <fileioc.h>
#include <stdint.h>
#include <graphx.h>
#include <keypadc.h>
#include <string.h>

int main(void) {
    uint8_t slot = ti_Open("Input", "r");
    char *data = ti_GetDataPtr(slot);
    unsigned int count;
    unsigned int dataLength = strlen(data);
    bool same = false;

    for (count = 0; count < dataLength - 14; count++) {
        same = false; // Reset for the beginning of a new one

        for (uint8_t i = 0; i < 14; i++) {
            for (uint8_t j = 1; j < 14 - i; j++) {
                if (data[count + i] == data[count + i + j]) {
                    same = true;
                }
            }
        }

        if (same == false) {
            break; // Exit on marker
        }
    }

    gfx_Begin();
    gfx_SetTextXY(0, 0);
    gfx_PrintInt(count + 14, 0); // Skip to the end of the four chars
    gfx_SetTextXY(0, 10);
    while (!kb_AnyKey());
    gfx_End();

    return 0;
}