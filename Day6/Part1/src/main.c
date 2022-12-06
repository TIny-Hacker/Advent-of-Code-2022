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

    for (count = 0; count < dataLength - 4; count++) {
        if ((data[count] == data[count + 1]) || (data[count] == data[count + 2]) || (data[count] == data[count + 3])) {
            continue;
        } else if ((data[count + 1] == data[count + 2]) || data[count + 1] == data[count + 3]) {
            continue;
        } else if (data[count + 2] == data[count + 3]) {
            continue;
        }

        break;  // Exit once we find the marker
    }

    gfx_Begin();
    gfx_SetTextXY(0, 0);
    gfx_PrintInt(count + 4, 0); // Skip to the end of the four chars
    gfx_SetTextXY(0, 10);
    while (!kb_AnyKey());
    gfx_End();

    return 0;
}