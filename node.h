#ifndef _NODE_H
#define _NODE_H

/*
* File:     node.h
* Purpose:  Header file for node.c
* Author:   Kerry Veenstra
*
* -----------------------
* DO NOT MODIFY THIS FILE
* -----------------------
*/

#include <inttypes.h>

typedef struct Node Node;

struct Node {
    uint8_t symbol;
    double weight;
    uint64_t code;
    uint8_t code_length;
    Node *left;
    Node *right;
};

Node *node_create(uint8_t symbol, uint32_t weight);
void node_free(Node **node);
void node_print_tree(Node *tree);

#endif
