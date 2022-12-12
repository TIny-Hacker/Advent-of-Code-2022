#include "asm/tools.h"

#include <fileioc.h>
#include <stdint.h>
#include <graphx.h>
#include <keypadc.h>

#include <debug.h>

#define MAX_ITEMS       32

typedef struct monkey_t {
    unsigned long items[MAX_ITEMS];               // Items the monkey is holding
    bool multOp;                            // Whether we add or multiply for the operation
    bool useOld;                            // Whether to use the operation on "old" or not
    unsigned int operation;                 // Operation that we add or multiply
    unsigned int divisible;                 // What to check the worry level by
    unsigned int trueMonkey;                // Number of monkey to send to
    unsigned int falseMonkey;               // Number of monkey to send to
    unsigned int activity;
} monkey_t;

static monkey_t *getMonkey(char *input) {
    monkey_t *newMonkey = malloc(sizeof(monkey_t));

    for (unsigned int i = 0; i < MAX_ITEMS; i++) {
        newMonkey->items[i] = '\0';
    }

    while (*input != 'S') { // Advance to "Starting items: "
        input++;
    }

    input += 16; // Skip to first number
    uint8_t currentSlot = 0;

    while (*input != '\0') {
        newMonkey->items[currentSlot] = getNumber(input);
        currentSlot++;

        while (*input != ' ' && *input != '\0') { // Advance to "Starting items: "
            input++;
        }

        if (*input != '\0') {
            input++;
        }
    }

    input += 24; // Skip to operation logic

    if (*input == '*') {
        newMonkey->multOp = true;   // Uses multiplication
    } else if (*input == '+') {
        newMonkey->multOp = false;  // Uses addition
    }

    input += 2;
    if (*input == 'o') {
        newMonkey->useOld = true;
        newMonkey->operation = 0;
    } else {
        newMonkey->useOld = false;
        newMonkey->operation = getNumber(input);
    }

    while (*input != 'y') { // Advance to "Test: divisible by"
        input++;
    }

    input += 2;

    newMonkey->divisible = getNumber(input);

    input += 31;    // Skip to throw logic
    newMonkey->trueMonkey = getNumber(input); // Temporary
    input += 32;
    newMonkey->falseMonkey = getNumber(input);

    newMonkey->activity = 0;

    return newMonkey;
}

static void appendItem(unsigned long item, monkey_t *newMonkey) {
    unsigned int newLocation = 0;

    while (newMonkey->items[newLocation] != 0) {
        newLocation++;
    }

    newMonkey->items[newLocation] = item;
}

int main(void) {
    unsigned int totalMonkeys = monkeyCount();
    monkey_t **monkeys = malloc(totalMonkeys * 3);

    unsigned int offset = 0;

    for (unsigned int i = 0; i < totalMonkeys; i++) {
        monkeys[i] = getMonkey(nextMonkey(&offset));
    }

    unsigned int itemSelected = 0;
    unsigned long worryLevel = 0;

    unsigned long commonMultiple = 1;
    for (unsigned int i = 0; i < totalMonkeys; i++) {
        commonMultiple *= monkeys[i]->divisible;
    }

    for (unsigned int cycle = 0; cycle < 10000; cycle++) {
        for (uint8_t currentMonkey = 0; currentMonkey < totalMonkeys; currentMonkey++) {
            //dbg_printf("\nCurrent Monkey\n------------------------\n\n%d\n", currentMonkey);
            itemSelected = 0;

            while (monkeys[currentMonkey]->items[itemSelected]) {
                worryLevel = monkeys[currentMonkey]->items[itemSelected];
                //dbg_printf("Item: %lu\n", worryLevel);

                if (monkeys[currentMonkey]->multOp) { // Multiply
                    if (monkeys[currentMonkey]->useOld) {
                        worryLevel = worryLevel * worryLevel;
                    } else {
                        worryLevel = worryLevel * monkeys[currentMonkey]->operation;
                    }
                } else {
                    if (monkeys[currentMonkey]->useOld) {
                        worryLevel = worryLevel + worryLevel;
                    } else {
                        worryLevel = worryLevel + monkeys[currentMonkey]->operation;
                    }
                }
                //dbg_printf("Item during inspection: %lu\n", worryLevel);

                //dbg_printf("Reduce %lu mod %lu\n", worryLevel, commonMultiple);
                worryLevel %= commonMultiple;
                //dbg_printf("Reduced: %lu\n", worryLevel);

                if (!(worryLevel % monkeys[currentMonkey]->divisible)) {    // True
                    //dbg_printf("Is divisible by %d\n", monkeys[currentMonkey]->divisible);
                    //dbg_printf("Passing to %d\n", monkeys[currentMonkey]->trueMonkey);
                    appendItem(worryLevel, monkeys[monkeys[currentMonkey]->trueMonkey]);
                    monkeys[currentMonkey]->items[itemSelected] = 0;
                } else {    // False
                    //dbg_printf("Is not divisible by %d\n", monkeys[currentMonkey]->divisible);
                    //dbg_printf("Passing to %d\n", monkeys[currentMonkey]->falseMonkey);
                    appendItem(worryLevel, monkeys[monkeys[currentMonkey]->falseMonkey]);
                    monkeys[currentMonkey]->items[itemSelected] = 0;
                }

                itemSelected++;
                monkeys[currentMonkey]->activity++;
                //dbg_printf("----------------------------------\n");
                //asm("push hl\n\tld hl, -1\n\tld (hl), 2\n\tpop hl");
                dbg_printf("Cycle %d\n", cycle);
            }
        }
    }

    unsigned int activeMonkeys[2] = {0};

    for (unsigned int currentMonkey = 0; currentMonkey < totalMonkeys; currentMonkey++) {
        if (monkeys[currentMonkey]->activity > activeMonkeys[1]) {
            activeMonkeys[0] = activeMonkeys[1];
            activeMonkeys[1] = monkeys[currentMonkey]->activity;
        } else if (monkeys[currentMonkey]->activity > activeMonkeys[0]) {
            activeMonkeys[0] = monkeys[currentMonkey]->activity;
        }
    }

    dbg_printf("%d * %d = %lu", activeMonkeys[0], activeMonkeys[1], activeMonkeys[0] * activeMonkeys[1]);

    gfx_Begin();
    gfx_SetTextXY(0, 0);
    gfx_PrintInt(activeMonkeys[0] * activeMonkeys[1], 1);
    while (!kb_AnyKey());
    gfx_End();

    return 0;
}
