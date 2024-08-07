#include "node.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/*
Create a Node and set its symbol and weight fields. Return a pointer to the new node. On error, return
NULL
*/
Node *node_create(uint8_t symbol, uint32_t weight) {
    Node *n = (Node *) malloc(sizeof(Node));
    if (n == NULL) {
        return NULL;
    }

    n->symbol = symbol;
    n->weight = weight;

    return n;
}

/*
Free the children of *pnode, free *pnode, and set *pnode to NULL
*/
void node_free(Node **pnode) {
    if (*pnode != NULL) {
        free(*pnode);
        *pnode = NULL;
    }
}

/*
This function is for diagnostics and debugging. You can print the tree in any way that you want. You
may use the recursive tree-printing routine below or write your own function. The provided function prints
on-screen a sideways view of the binary tree using text characters
*/
void node_print_node(Node *tree, char ch, int indentation) {
    if (tree == NULL) {
        return;
    }
    node_print_node(tree->right, '/', indentation + 3);
    printf("%*cweight = %d", indentation + 1, ch, (int) tree->weight);
    if (tree->left == NULL && tree->right == NULL) {
        if (' ' <= tree->symbol && tree->symbol <= '~') {
            printf(", symbol = '%c'", tree->symbol);
        } else {
            printf(", symbol = 0x%02x", tree->symbol);
        }
    }

    printf("\n");
    node_print_node(tree->left, '\\', indentation + 3);
}

void node_print_tree(Node *tree) {
    node_print_node(tree, '<', 2);
}
