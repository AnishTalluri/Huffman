#include "pq.h"

#include "node.h"

#include <stdint.h>
#include <stdio.h>

typedef struct ListElement ListElement;

struct ListElement {
    Node *tree;
    ListElement *next;
};

struct PriorityQueue {
    ListElement *list;
};

/*
Allocate a PriorityQueue object and return a pointer to it. If there’s an error, return NULL
*/
PriorityQueue *pq_create(void) {
    PriorityQueue *p = (PriorityQueue *) malloc(sizeof(PriorityQueue));
    if (p == NULL) {
        return NULL;
    }

    p->list = NULL;

    return p;
}

/*
Call free() on *q, and then set *q = NULL
*/
void pq_free(PriorityQueue **q) {
    if (*q != NULL) {
        ListElement *curr = (*q)->list;
        ListElement *next;
        while (curr != NULL) {
            next = curr->next;
            free(curr);
            curr = next;
        }

        free(*q);
        *q = NULL;
    }
}

/*
We indicate an empty queue by storing NULL in the queue’s list field. Return true if that’s the case
*/
bool pq_is_empty(PriorityQueue *q) {
    return (q->list == NULL);
}

bool pq_size_is_1(PriorityQueue *q) {
    return (q->list != NULL && q->list->next == NULL);
}

bool pq_less_than(ListElement *e1, ListElement *e2) {
    if (e1->tree->weight < e2->tree->weight) {
        return true;
    } else if (e1->tree->weight == e2->tree->weight) {
        return (e1->tree->symbol < e2->tree->symbol);
    }
    return false;
}

void enqueue(PriorityQueue *q, Node *tree) {
    ListElement *new_element = (ListElement *) malloc(sizeof(ListElement));
    if (new_element == NULL) {
        exit(1);
    }

    new_element->tree = tree;

    if (q->list == NULL) {
        q->list = new_element;
    } else if (pq_less_than(new_element, q->list)) {
        new_element->next = q->list;

        q->list = new_element;
    } else {
        ListElement *current_element = q->list;
        while (current_element->next != NULL && !pq_less_than(new_element, current_element->next)) {
            current_element = current_element->next;
        }

        new_element->next = current_element->next;
        current_element->next = new_element;
    }
}

Node *dequeue(PriorityQueue *q) {
    if (q->list == NULL) {
        exit(1);
    }

    Node *result = q->list->tree;
    ListElement *temp = q->list;
    q->list = q->list->next;
    free(temp);

    return result;
}

void pq_print(PriorityQueue *q) {
    assert(q != NULL);
    ListElement *e = q->list;
    int position = 1;
    while (e != NULL) {
        if (position++ == 1) {
            printf("=============================================\n");
        } else {
            printf("---------------------------------------------\n");
        }
        node_print_tree(e->tree);
        e = e->next;
    }
    printf("=============================================\n");
}
