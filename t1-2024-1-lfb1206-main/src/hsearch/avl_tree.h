#pragma once
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "heads.h"

typedef struct node {
    int priority;
    int head_index;
    int height;
    int left_height;
    int right_height;
    struct node* left_node;
    struct node* right_node;
    struct node* same_priority_next;
    int quantity_same_priority;
} Node;

Node* node_create(int priority, int head_index);

void add_same_priority_node(Node* node, Node* new_node);

void free_node(Node* node);

int height(Node* node);

int balance(Node* node);

Node* right_rotate(Node* y);

Node* left_rotate(Node* x);

Node* tree_insert(Node* root, Node* node);

void tree_free(Node* root);

Node* search_node(Node* root, int priority);

void search_nodes_within_range(Node* root, int min_priority, int max_priority, Node **nodes, int *nodes_quantity, int *nodes_index);

void search_nodes_within_circular_range(Node* root, int radious, int center_x, int center_y, int x, Node **nodes, int *nodes_quantity, int *nodes_index);
