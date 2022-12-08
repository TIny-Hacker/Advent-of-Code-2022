#include "asm/tools.h"

#include <fileioc.h>
#include <stdint.h>

#include <debug.h>

#define MAX_CHILDREN    16 // No more than this please

// This is my first time doing trees and all that nonsense, so don't judge!
// Lots of comments because commenting helps me understand what I'm doing better

long totalUsed = 0;
long directory = 0;

typedef struct node_t {
    long int size; // Total size of files in the directory "node"
    uint8_t noChildren; // Number of child directory "nodes" (I don't think there will be very many per each)
    struct node_t *prev; // Address of previous nodes
    struct node_t *children[MAX_CHILDREN]; // Addresses of child nodes
} node_t;

static node_t *append(node_t *parent) {
    node_t *child = malloc(sizeof(node_t)); // Create the child

    if (child != NULL) { // Ensure that the child was created properly
        child->size = 0;
        child->noChildren = 0;

        if (parent != NULL) {
            parent->children[parent->noChildren] = child; // Link child to parent
            parent->noChildren++;
            child->prev = parent; // Link parent to child
        }
    }

    return child; // Pass back pointer to child
}

static void findDeleteDir(node_t *node) {
    if (node == NULL) {
        return;
    }

    for (uint8_t i = 0; i < node->noChildren; i++) {
        findDeleteDir(node->children[i]);
    }

    if ((node->size >= totalUsed - 40000000) && (node->size < directory || directory == 0)) {
        directory = node->size;
    }
}

static void parentSize(node_t *node, long size) {
    while (node->prev != NULL) {
        node->size += size;
        node = node->prev;
    }
    node->size += size; // Fix for root
}

int main(void) {
    node_t *current = NULL;
    node_t *root = malloc(sizeof(node_t));
    root->size = 0;
    root->noChildren = 0;
    root->prev = NULL;

    char *instruction;

    unsigned int offset = 0;

    uint8_t slot = ti_Open("Input", "r");
    unsigned int inputSize = ti_GetSize(slot);
    ti_Close(slot);

    while (offset < inputSize) {
        instruction = getLine(&offset);
        if (instruction[0] == '$') { // This means it is a command
            if (instruction[2] == 'c') {
                switch (instruction[5]) {
                    case '/': // Switch to root
                        current = root;
                        break;
                    case '.': // Switch to previous directory
                        current = current->prev;
                        break;
                    default:
                        current = append(current); // `cd`ing gives us a new directory
                }
            } else {
                continue; // We don't really need to know about ls
            }
        } else if (instruction[0] != 'd') { // Filesize
            parentSize(current, getNumber(instruction));
        }
    }

    totalUsed = root->size;
    findDeleteDir(root);
    dbg_printf("%lu\n", directory);

    return 0;
}

