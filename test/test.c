#include "linked_list.h"
#include "stdio.h"

static void obj_print(basic_obj_t obj) { printf("%d ", obj.i); }

static void test1() {
    linked_list_t l;
    linked_list_init(&l);

    linked_list_insert_tail(&l)->data.i = 1;
    linked_list_insert_tail(&l)->data.i = 2;
    linked_list_insert_tail(&l)->data.i = 3;

    linked_list_insert_head(&l)->data.i = -1;
    linked_list_insert_head(&l)->data.i = -2;

    printf("%d\n", linked_list_index_of(&l, *(basic_obj_t[]){{.i = 3}}, NULL));

    linked_list_insert(&l, l.lenth)->data.i = 5;

    linked_list_delete(&l, l.lenth - 1, NULL);
    linked_list_insert_tail(&l)->data.i = 7;

    linked_list_foreach(&l, obj_print);
    printf("\n");

    linked_list_destory(&l);
}

static void test2() {
    dlinked_list_t l;
    dlinked_list_init(&l);

    dlinked_list_insert_tail(&l)->data.i = 1;
    dlinked_list_insert_tail(&l)->data.i = 2;
    dlinked_list_insert_tail(&l)->data.i = 3;

    dlinked_list_insert_head(&l)->data.i = -1;
    dlinked_list_insert_head(&l)->data.i = -2;

    printf("%d\n", dlinked_list_index_of(&l, *(basic_obj_t[]){{.i = 3}}, NULL));

    dlinked_list_insert(&l, l.lenth)->data.i = 5;

    dlinked_list_delete(&l, l.lenth - 1, NULL);
    dlinked_list_insert_tail(&l)->data.i = 7;
    dlinked_list_insert(&l, 3)->data.i = 9;

    dlinked_list_foreach(&l, obj_print);
    printf("\n");

    dlinked_list_destory(&l);
}

int main() {
    test1();
    test2();
    return 0;
}
