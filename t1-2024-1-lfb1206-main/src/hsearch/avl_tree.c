#include "avl_tree.h"

Node* node_create(int priority, int head_index)
{
    Node* node = (Node*)malloc(sizeof(Node));
    node->priority = priority;
    node->head_index = head_index;
    node->height = 1;
    node->left_height = 0;
    node->right_height = 0;
    node->left_node = NULL;
    node->right_node = NULL;
    node->same_priority_next = NULL;
    node->quantity_same_priority = 1;
    return node;
}

void add_same_priority_node(Node* node, Node* new_node)
{
    if (node->same_priority_next == NULL)
        node->same_priority_next = new_node;
    else 
        add_same_priority_node(node->same_priority_next, new_node);
}

void free_node(Node* node)
{
    if (node == NULL)
        return;
    free_node(node->same_priority_next);
    free(node);
}

int height(Node* node)
{
    if (node == NULL)
        return 0;
    return node->height;
}

Node* right_rotate(Node* x) {
    Node* y = x->left_node;
    Node* z = y->right_node;
    y->right_node = x;
    x->left_node = z;
    x->height = (height(x->left_node) > height(x->right_node) ? height(x->left_node) : height(x->right_node)) + 1;
    y->height =(height(y->left_node) > height(y->right_node) ? height(y->left_node) : height(y->right_node)) + 1;
    return y;
}

Node* left_rotate(Node* x) {
    Node* y = x->right_node;
    Node* z = y->left_node;
    y->left_node = x;
    x->right_node = z;
    x->height =(height(x->left_node) > height(x->right_node) ? height(x->left_node) : height(x->right_node)) + 1;
    y->height = (height(y->left_node) > height(y->right_node) ? height(y->left_node) : height(y->right_node)) + 1;
    return y;
}

Node* tree_insert(Node* root, Node* node)
{
    if (root == NULL)
        return node;
    else if (root->priority < node->priority)
        root->right_node = tree_insert(root->right_node, node);
    else if (root->priority > node->priority)
        root->left_node = tree_insert(root->left_node, node);
    else {
        add_same_priority_node(root, node);
        root->quantity_same_priority++;
        return root;
    }
    root->height = (height(root->left_node) > height(root->right_node) ? height(root->left_node) : height(root->right_node)) + 1;
    int balance_factor = 0;
    if (root->left_node != NULL && root->right_node != NULL)
        balance_factor = root->right_node->height - root->left_node->height;
    else if (root->left_node == NULL)
        balance_factor = root->right_node->height;
    else if (root->right_node == NULL)
        balance_factor = -root->left_node->height;

    if (balance_factor < -2 && node->priority < root->left_node->priority)
        return right_rotate(root);

    if (balance_factor > 2 && node->priority > root->right_node->priority)
        return left_rotate(root);

    if (balance_factor < -2 && node->priority > root->left_node->priority) {
        root->left_node = left_rotate(root->left_node);
        return right_rotate(root);
    }

    if (balance_factor > 2 && node->priority < root->right_node->priority) {
        root->right_node = right_rotate(root->right_node);
        return left_rotate(root);
    }
    return root;
}

void tree_free(Node* root)
{
    if (root == NULL)
        return;
    else {
        tree_free(root->left_node);
        tree_free(root->right_node);
        free_node(root);
    }
}

Node* search_node(Node* root, int priority) {
    if (root == NULL)
        return NULL;
    else if (root->priority == priority)
        return root;
    else if (root->priority < priority)
        return search_node(root->right_node, priority);
    else
        return search_node(root->left_node, priority);
}

void search_nodes_within_range(Node* root, int min_priority, int max_priority, Node **nodes, int *nodes_quantity, int *nodes_index)
{
    if (root == NULL)
        return;
    if (min_priority <= root->priority)
        search_nodes_within_range(root->left_node, min_priority, max_priority, nodes, nodes_quantity, nodes_index);
    if (min_priority <= root->priority && root->priority < max_priority) {
        nodes[*nodes_index] = root;
        *nodes_quantity += root->quantity_same_priority;
        *nodes_index += 1;
    }
    if (root->priority < max_priority)
        search_nodes_within_range(root->right_node, min_priority, max_priority, nodes, nodes_quantity, nodes_index);
}

void search_nodes_within_circular_range(Node* root, int radious, int center_x, int center_y, int x, Node **nodes, int *nodes_quantity, int *nodes_index)
{
    if (root == NULL)
        return;
    if (center_y - radious <= root->priority)
        search_nodes_within_circular_range(root->left_node, radious, center_x, center_y, x, nodes, nodes_quantity, nodes_index);
    if (center_y - radious <= root->priority && root->priority <= center_y + radious && ((center_x - x) * (center_x - x) + (center_y - root->priority) * (center_y - root->priority) <= (radious * radious))) {
        nodes[*nodes_index] = root;
        *nodes_quantity += root->quantity_same_priority;
        *nodes_index += 1;
    }
    if (root->priority <= center_y + radious)
        search_nodes_within_circular_range(root->right_node, radious, center_x, center_y, x, nodes, nodes_quantity, nodes_index);
}