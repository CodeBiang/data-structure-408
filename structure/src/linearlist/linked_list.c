#include "linked_list.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

bool linked_list_init(linked_list_t* l) {
    l->head = l->tail = NULL;
    l->lenth = 0;
}

void linked_list_destory(linked_list_t* l) {
    linked_node_t* cur = l->head;
    linked_node_t* temp;

    while (cur) {
        temp = cur->next;

        free(cur);

        cur = temp;
    }
}

linked_node_t* linked_list_insert_head(linked_list_t* l) {
    linked_node_t* node = (linked_node_t*) malloc(sizeof(linked_node_t));

    if (!node) return NULL;

    memset(node, 0, sizeof(linked_node_t));

    if (!l->head) {
        l->tail = l->head = node;
    } else {
        node->next = l->head;
        l->head = node;
    }

    l->lenth++;

    return node;
}

linked_node_t* linked_list_insert_tail(linked_list_t* l) {
    linked_node_t* node = (linked_node_t*) malloc(sizeof(linked_node_t));

    if (!node) return NULL;

    memset(node, 0, sizeof(linked_node_t));

    if (!l->tail) {
        l->tail = l->head = node;
    } else {
        l->tail->next = node;
        l->tail = node;
    }

    l->lenth++;

    return node;
}

linked_node_t* linked_list_insert(linked_list_t* l, int idx) {
    if (idx < 0 || idx > l->lenth) return NULL;

    if (idx == 0)
        return linked_list_insert_head(l);
    else if (idx == l->lenth)
        return linked_list_insert_tail(l);

    linked_node_t* node = (linked_node_t*) malloc(sizeof(linked_node_t));
    if (!node) return node;

    linked_node_t* cur = l->head;

    while (--idx) {
        cur = cur->next;
    }

    assert(cur);

    linked_node_t* temp = cur->next;
    cur->next = node;
    node->next = temp;

    l->lenth++;

    return node;
}

bool linked_list_delete(linked_list_t* l, int idx, basic_obj_t* deleted_item) {
    if (idx < 0 || idx >= l->lenth) return false;

    linked_node_t* temp = NULL;

    if (idx == 0) {
        temp = l->head;
        // 只有一个元素
        if (l->head == l->tail) {
            l->head = l->tail = NULL;
        } else
           l->head = l->head->next;
    }

    // 头尾删除
    if (temp) {
        if (deleted_item) memcpy(deleted_item, &temp->data, sizeof(basic_obj_t));
        free(temp);
        l->lenth--;
        return true;
    }

    linked_node_t* cur = l->head;

    while (--idx) {
        cur = cur->next;
    }

    temp = cur->next;
    if (temp == l->tail) 
        l->tail = cur;
    cur->next = temp->next;
    if (deleted_item) memcpy(deleted_item, &temp->data, sizeof(basic_obj_t));
    free(temp);

    return true;
}

static bool memequals(const basic_obj_t a, const basic_obj_t b) {
    return memcmp(&a, &b, sizeof(basic_obj_t)) == 0;
}

int linked_list_index_of(linked_list_t* l, basic_obj_t elem, basic_obj_equals_fn equals) {
    basic_obj_equals_fn eq = equals ? equals : memequals;

    int idx = 0;
    linked_node_t* node = l->head;

    while (node) {
        if (eq(node->data, elem)) {
            return idx;
        }
        node = node->next;
        idx++;
    }

    return -1;
}

linked_node_t* linked_list_at(linked_list_t* l, int idx) {
    if (idx < 0 || idx >= l->lenth) return NULL;

    linked_node_t* node = l->head;

    while (--idx) {
        node = node->next;
    }

    return node;
}

void linked_list_foreach(linked_list_t* l, basic_obj_accept_fn accept) {
    linked_node_t* node = l->head;

    while (node) {
        accept(node->data);
        node = node->next;
    }
}