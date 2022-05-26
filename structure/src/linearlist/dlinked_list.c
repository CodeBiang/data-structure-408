#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "linked_list.h"

bool dlinked_list_init(dlinked_list_t* l) {
    l->head = l->tail = NULL;
    l->lenth = 0;
}

void dlinked_list_destory(dlinked_list_t* l) {
    dlinked_node_t* cur = l->head;
    dlinked_node_t* temp;

    while (cur) {
        temp = cur->next;

        free(cur);

        cur = temp;
    }
}

dlinked_node_t* dlinked_list_insert_head(dlinked_list_t* l) {
    dlinked_node_t* node = (dlinked_node_t*) malloc(sizeof(dlinked_node_t));

    if (!node) return NULL;

    memset(node, 0, sizeof(dlinked_node_t));

    if (!l->head) {
        l->tail = l->head = node;
    } else {
        node->next = l->head;
        l->head->prev = node;
        l->head = node;
    }

    l->lenth++;

    return node;
}

dlinked_node_t* dlinked_list_insert_tail(dlinked_list_t* l) {
    dlinked_node_t* node = (dlinked_node_t*) malloc(sizeof(dlinked_node_t));

    if (!node) return NULL;

    memset(node, 0, sizeof(dlinked_node_t));

    if (!l->tail) {
        l->tail = l->head = node;
    } else {
        l->tail->next = node;
        node->prev = l->tail;
        l->tail = node;
    }

    l->lenth++;

    return node;
}

dlinked_node_t* dlinked_list_insert(dlinked_list_t* l, int idx) {
    if (idx < 0 || idx > l->lenth) return NULL;

    if (idx == 0)
        return dlinked_list_insert_head(l);
    else if (idx == l->lenth)
        return dlinked_list_insert_tail(l);

    dlinked_node_t* node = (dlinked_node_t*) malloc(sizeof(dlinked_node_t));
    if (!node) return node;

    bool from_head = idx < l->lenth / 2;

    dlinked_node_t* cur;
    if (from_head) {
        cur = l->head;
        while (--idx) cur = cur->next;
    } else {
        cur = l->tail;
        while (++idx <= l->lenth) cur = cur->prev;
    }

    assert(cur);

    dlinked_node_t* temp = cur->next;
    cur->next = node;
    node->next = temp;

    l->lenth++;

    return node;
}

bool dlinked_list_delete(dlinked_list_t* l, int idx, basic_obj_t* deleted_item) {
    if (idx < 0 || idx >= l->lenth || l->lenth == 0) return false;

    dlinked_node_t* temp = NULL;

    if (idx == 0) {
        temp = l->head;

        if (l->head == l->tail)
            l->head = l->tail = NULL;
        else {
            l->head = l->head->next;
            l->head->prev = NULL;
        }
    } else if (idx == l->lenth - 1) {
        temp = l->tail;
        l->tail = l->tail->prev;
        l->tail->next = NULL;
    }

    // 头尾删除
    if (temp) {
        if (deleted_item) memcpy(deleted_item, &temp->data, sizeof(basic_obj_t));
        free(temp);
        l->lenth--;
        return true;
    }

    dlinked_node_t* cur = l->head;

    while (--idx) {
        cur = cur->next;
    }

    temp = cur->next;
    cur->next = temp->next;
    temp->next->prev = cur;
    if (deleted_item) memcpy(deleted_item, &temp->data, sizeof(basic_obj_t));
    free(temp);

    return true;
}

static bool memequals(const basic_obj_t a, const basic_obj_t b) {
    return memcmp(&a, &b, sizeof(basic_obj_t)) == 0;
}

int dlinked_list_index_of(dlinked_list_t* l, basic_obj_t elem, basic_obj_equals_fn equals) {
    basic_obj_equals_fn eq = equals ? equals : memequals;

    int idx = 0;
    dlinked_node_t* node = l->head;

    while (node) {
        if (eq(node->data, elem)) {
            return idx;
        }
        node = node->next;
        idx++;
    }

    return -1;
}

dlinked_node_t* dlinked_list_at(dlinked_list_t* l, int idx) {
    if (idx < 0 || idx >= l->lenth) return NULL;

    dlinked_node_t* node = l->head;

    while (--idx) {
        node = node->next;
    }

    return node;
}

void dlinked_list_foreach(dlinked_list_t* l, basic_obj_accept_fn accept) {
    dlinked_node_t* node = l->head;

    while (node) {
        accept(node->data);
        node = node->next;
    }
}