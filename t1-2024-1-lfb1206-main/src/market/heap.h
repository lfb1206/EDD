#pragma once
#include <stdlib.h>

typedef struct node {
    int user;
    int amount;
    int priority;
} Node;

typedef struct heap {
    Node **nodes;
    int size;
    int capacity;
} Heap;


Heap *heap_create(int capacity);

void heap_free(Heap *heap);

void heap_insert(Heap *heap, int user, int amount, int priority);

Node *heap_min(Heap* heap);

void heap_extract_min(Heap *heap);

void percolate_down(Heap *heap, int index);

void percolate_up(Heap *heap, int index);

