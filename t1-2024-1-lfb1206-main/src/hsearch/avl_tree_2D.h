#pragma once
#include "avl_tree.h"

typedef struct node_to_tree {
    int priority;
    int height;
    int left_height;
    int right_height;
    struct node_to_tree* left_node;
    struct node_to_tree* right_node;
    struct node* avl_tree;
} NodeToTree;

NodeToTree* node_to_tree_create(int priority, int district_priority, int head_index);

int node_to_treeheight(NodeToTree* node);

int node_to_treebalance(NodeToTree* node);

NodeToTree* node_to_treeright_rotate(NodeToTree* y);

NodeToTree* node_to_treeleft_rotate(NodeToTree* x);

NodeToTree* node_to_tree_insert(NodeToTree* root, NodeToTree* node);

void node_to_tree_free(NodeToTree* root);

Node* search_node_to_tree(NodeToTree* root, int year_priority, int district_priority);

void search_node_to_tree_within_range(NodeToTree* root, int center_x, int center_y, int radius, Node **nodes, int *nodes_quantity, int *nodes_index);
