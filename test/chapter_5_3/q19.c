/**
 * @file q19.c
 * @author your name (you@domain.com)
 * @brief 求 WPL
 *
 * 关于WPL
 *
 * WPL求法是指所有叶节点的 权值×路径深度(层数-1) 的和
 *
 * 如果是霍夫曼树，非叶子节点权值之和就是WPL值
 *
 * 用一个霍夫曼树为例
 * 如：
 * 序列： 1 3 5 10 11 16 14 6 22 17
 *                     105
 *         43                           62
 *     21     [22]              29                33
 * [10]  [11]              [14]     15        [16]   [17]
 *                              [6]    9
 *                                   4  [5]
 *                                 [1 3]
 * wpl = 10 * 3 + 11 * 3 + 22 * 2 + 14 * 3 + 6 * 4 + 5 * 5 + 1 * 6 + 3 * 6 + 16
 * * 3 + 17 * 3 = 30 + 33 + 44 + 42 + 24 + 25 + 6 + 18 + 48 + 51 = 321 = 21 + 43
 * + 105 + 62 + 29 + 15 + 9 + 4 + 33
 */
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct node_s node_t;
struct node_s {
    node_t* left;
    node_t* right;
    int weight;
};

static node_t* node_create(int weight) {
    node_t* node = (node_t*) malloc(sizeof(node_t));
    node->left = node->right = NULL;
    node->weight = weight;
    return node;
}

int wpl(node_t* node, int deep) {
    if (!node) return 0;
    if (!node->left && !node->right) {
        return deep * node->weight;
    } else {
        int val = 0;
        val += wpl(node->left, deep + 1);
        val += wpl(node->right, deep + 1);
        return val;
    }
}

int main() {
    node_t* root = node_create(105);
    root->left = node_create(43);
    root->left->left = node_create(21);
    root->left->left->left = node_create(10);
    root->left->left->right = node_create(11);
    root->left->right = node_create(22);
    root->right = node_create(62);
    root->right->left = node_create(29);
    root->right->left->left = node_create(14);
    root->right->left->right = node_create(15);
    root->right->left->right->left = node_create(6);
    root->right->left->right->right = node_create(9);
    root->right->left->right->right->left = node_create(4);
    root->right->left->right->right->right = node_create(5);
    root->right->left->right->right->left->left = node_create(1);
    root->right->left->right->right->left->right = node_create(3);
    root->right->right = node_create(33);
    root->right->right->left = node_create(16);
    root->right->right->right = node_create(17);

    printf("wpl %d\n", wpl(root, 0));
    return 0;
}
