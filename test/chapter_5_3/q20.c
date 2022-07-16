#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node_s node_t;
struct node_s {
    node_t* left;
    node_t* right;
    char expr[10];
};

node_t* node_create(const char* expr) {
    node_t* node = (node_t*) malloc(sizeof(node_t));
    node->left = node->right = NULL;
    strncpy(node->expr, expr, 10);
    return node;
}

void expression_print(node_t* node, node_t* root) {
    if (!node) return;
    bool brackets = node != root && (node->left || node->right);
    if (brackets) printf("(");
    expression_print(node->left, root);
    printf("%s", node->expr);
    expression_print(node->right, root);
    if (brackets) printf(")");
}

int main() {
    node_t* root = node_create("+");
    root->left = node_create("*");
    root->left->left = node_create("a");
    root->left->right = node_create("b");
    root->right = node_create("-");
    root->right->right = node_create("-");
    root->right->right->left = node_create("c");
    root->right->right->right = node_create("d");

    expression_print(root, root);

    return 0; 
}
