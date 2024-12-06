#include "avl_tree_2D.h"

NodeToTree* node_to_tree_create(int year_priority, int district_priority, int head_index)
{
    NodeToTree* node = (NodeToTree*)malloc(sizeof(NodeToTree));
    node->priority = year_priority;
    node->height = 1;
    node->left_height = 0;
    node->right_height = 0;
    node->left_node = NULL;
    node->right_node = NULL;
    node->avl_tree = node_create(district_priority, head_index);
    return node;
}

int node_to_tree_height(NodeToTree* node)
{
    if (node == NULL)
        return 0;
    return node->height;
}

NodeToTree* node_to_tree_right_rotate(NodeToTree* y) {
    NodeToTree* x = y->left_node;
    NodeToTree* z = x->right_node;
    x->right_node = y;
    y->left_node = z;
    y->height = (node_to_tree_height(y->left_node) > node_to_tree_height(y->right_node) ? node_to_tree_height(y->left_node) : node_to_tree_height(y->right_node)) + 1;
    x->height =(node_to_tree_height(x->left_node) > node_to_tree_height(x->right_node) ? node_to_tree_height(x->left_node) : node_to_tree_height(x->right_node)) + 1;
    return x;
}

NodeToTree* node_to_tree_left_rotate(NodeToTree* x) {
    NodeToTree* y = x->right_node;
    NodeToTree* z = y->left_node;
    y->left_node = x;
    x->right_node = z;
    x->height =(node_to_tree_height(x->left_node) > node_to_tree_height(x->right_node) ? node_to_tree_height(x->left_node) : node_to_tree_height(x->right_node)) + 1;
    y->height = (node_to_tree_height(y->left_node) > node_to_tree_height(y->right_node) ? node_to_tree_height(y->left_node) : node_to_tree_height(y->right_node)) + 1;
    return y;
}

NodeToTree* node_to_tree_insert(NodeToTree* root, NodeToTree* node)
{
    if (root == NULL)
        return node;
    else if (root->priority < node->priority)
        root->right_node = node_to_tree_insert(root->right_node, node);
    else if (root->priority > node->priority)
        root->left_node = node_to_tree_insert(root->left_node, node);
    else {
        root->avl_tree = tree_insert(root->avl_tree, node->avl_tree);
        free(node);
        return root;
    }
    root->height = (node_to_tree_height(root->left_node) > node_to_tree_height(root->right_node) ? node_to_tree_height(root->left_node) : node_to_tree_height(root->right_node)) + 1;
    int balance_factor = 0;
    if (root->left_node != NULL && root->right_node != NULL)
        balance_factor = root->right_node->height - root->left_node->height;
    else if (root->left_node == NULL)
        balance_factor = root->right_node->height;
    else if (root->right_node == NULL)
        balance_factor = -root->left_node->height;
    if (balance_factor < -2 && node->priority < root->left_node->priority)
        return node_to_tree_right_rotate(root);
    if (balance_factor > 2 && node->priority > root->right_node->priority)
        return node_to_tree_left_rotate(root);
    if (balance_factor < -2 && node->priority > root->left_node->priority) {
        root->left_node = node_to_tree_left_rotate(root->left_node);
        return node_to_tree_right_rotate(root);
    }
    if (balance_factor > 2 && node->priority < root->right_node->priority) {
        root->right_node = node_to_tree_right_rotate(root->right_node);
        return node_to_tree_left_rotate(root);
    }
    return root;
}

void node_to_tree_free(NodeToTree* root)
{
    if (root == NULL)
        return;
    else {
        node_to_tree_free(root->left_node);
        node_to_tree_free(root->right_node);
        tree_free(root->avl_tree);
        free(root);
    }
}

Node* search_node_to_tree(NodeToTree* root, int year_priority, int district_priority) {
    if (root == NULL)
        return NULL;
    else if (root->priority == year_priority)
        return search_node(root->avl_tree, district_priority);
    else if (root->priority < year_priority)
        return search_node_to_tree(root->right_node, year_priority, district_priority);
    else 
        return search_node_to_tree(root->left_node, year_priority, district_priority);
}

void search_node_to_tree_within_range(NodeToTree* root, int center_x, int center_y, int radius, Node **nodes, int *nodes_quantity, int *nodes_index)
{
    if (root == NULL)
        return;
    if ((center_x - radius) <= root->priority)
        search_node_to_tree_within_range(root->left_node, center_x, center_y, radius, nodes, nodes_quantity, nodes_index);
    if ((center_x - radius) <= root->priority && root->priority <= (center_x + radius))
        search_nodes_within_circular_range(root->avl_tree, radius, center_x, center_y, root->priority, nodes, nodes_quantity, nodes_index);
    if (root->priority <= (center_x + radius))
        search_node_to_tree_within_range(root->right_node, center_x, center_y, radius, nodes, nodes_quantity, nodes_index);
}
