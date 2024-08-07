#ifndef _PQ_H
#define _PQ_H

/*
* File:     pq.h
* Purpose:  Header file for PriorityQueue using a linked list.
* Author:   Kerry Veenstra
*
* -----------------------
* DO NOT MODIFY THIS FILE
* -----------------------
*/

#include "node.h"

#include <assert.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct PriorityQueue PriorityQueue;

PriorityQueue *pq_create(void);
void pq_free(PriorityQueue **q);
bool pq_is_empty(PriorityQueue *q);
bool pq_size_is_1(PriorityQueue *q);
void enqueue(PriorityQueue *q, Node *tree);
Node *dequeue(PriorityQueue *q);
void pq_print(PriorityQueue *q);

#endif
