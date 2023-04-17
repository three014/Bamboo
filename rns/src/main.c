#include "main.h"
#include "object.h"

void vec_test();
void string_test();
void avltree_test();
void vec_test_again();
void set_test();
void avl_tree_map_test();

int main(int argc, char **argv) {
    // process args
    Settings *rns_settings = settings_from_args(argc, argv);


    if (rns_settings != NULL) {
        free(rns_settings);
        rns_settings = NULL;
    } else {
        return 1;
    }

    string_test();
    printf("--------------------------------------------------------------------\n");
    vec_test();
    printf("--------------------------------------------------------------------\n");
    avltree_test();
    printf("--------------------------------------------------------------------\n");
    set_test();
    printf("--------------------------------------------------------------------\n");
    avl_tree_map_test();
    // vec_test_again();
    return 0;
}

void string_test() {
    char *e = "Hello!";
    printf("char *e = %p\n", e);
    char *s = "Hola!";
    printf("char *s = %p\n", s);
    char *g = "guwah";
    printf("char *g = %p\n", g);

    Vec *str_vec = Vec_with_capacity(2);
    Vec_push(str_vec, Obj_from_str(e));
    Vec_push(str_vec, Obj_from_str(s));
    Vec_push(str_vec, Obj_from_str(g));

    Iter_for_each_obj(Vec_iter(str_vec, false), ^ void (ObjWrap item) {
        char *str = Obj_to_str(item);
        printf("char *str = %p\n", str);
        printf("%s\n", str);

    });
    
    Vec_delete(&str_vec);
}

typedef struct guwah {
    char y;
    int x;
    double z;
} Test;

static Ordering cmp_test(const void *test1, const void *test2) {
    const Test *t1 = test1;
    const Test *t2 = test2;

    if (t1->x == t2->x) {
        if (t1->y == t2->y) {
            double cmp = t1->z - t2->z;
            if (cmp < 0.001 && cmp > -0.001) {
                return Equal;
            }
            if (cmp > 0.001) {
                return Greater;
            }
            return Less;
        }
        if (t1->y > t2->y) {
            return Greater;
        }
        return Less;
    }
    if (t1->x > t2->x) {
        return Greater;
    }
    return Less;
}

void Test_free(Test **self) { free(*self); }

void vec_test() {
    
    __attribute__((cleanup(Test_free))) Test *t = malloc(sizeof *t);
    t->x = 4;
    t->y = 'c';
    t->z = 3.4;
    __attribute__((cleanup(Test_free))) Test *t1 = malloc(sizeof *t1);
    t1->x = 3;
    t1->y = 'c';
    t1->z = 6.432;
    __attribute__((cleanup(Test_free))) Test *t2 = malloc(sizeof *t2);
    t2->x = 6;
    t2->y = 's';
    t2->z = 46.2;
    __attribute__((cleanup(Test_free))) Test *t3 = malloc(sizeof *t3);
    t3->x = 5;
    t3->y = 'g';
    t3->z = 3.67;

    __attribute__((cleanup(Vec_delete))) Vec *test_vec = Vec_with_capacity(2);
    Vec_push(test_vec, Obj_from_ptr(t));
    Vec_push(test_vec, Obj_from_ptr(t1));
    Vec_push(test_vec, Obj_from_ptr(t2));
    Vec_push(test_vec, Obj_from_ptr(t3));

    void *test_ptr = Obj_to_ptr(Obj_from_ptr(t));
    printf("t: %p\n", t);
    printf("Testing Obj_to_ptr(Obj_from_ptr(t)): %p\n", test_ptr);

    void *test_ptr1 = Obj_to_ptr(Obj_from_ptr(t1));
    printf("t1: %p\n", t1);
    printf("Testing Obj_to_ptr(Obj_from_ptr(t1)): %p\n", test_ptr1);


    Iterator mapped_vec_iter = Iter_map(Vec_iter(test_vec, false), ^ ObjWrap (ObjWrap item) {
        char *str = malloc(64);
        Test *test = Obj_to_ptr(item);
        printf("test ptr: %p\n", test);
        memset(str, 0, 64);
        sprintf(str, "Test: x = %d, y = %c, z = %lf\n", test->x, test->y, test->z);
        str[63] = '\0';
        return Obj_from_str(str);
    });
   
    Iter_for_each_obj(mapped_vec_iter, ^ void (ObjWrap item) {
        char *str = Obj_to_str(item);
        printf("%s\n", str);
        free(str);
    });

    OrderingVTable test_ord = {
        .cmp = cmp_test,
    };

    Vec *vec2 = Iter_collect(Vec_iter(test_vec, false), Vec_constr());

    Vec_delete(&vec2);
}

void avltree_test() {
    AvlTreeSet *tree = AvlTreeSet_new(&ordering_str);
    char *x = "Today is a good day for to have to have a good day";
    char *y = "Tomorrow is a gift";
    char *z = "That's why yesterday is a yesterday";
    AvlTreeSet_insert(tree, y);
    AvlTreeSet_insert(tree, x);
    AvlTreeSet_insert(tree, z);

    Iter_for_each_obj(AvlTreeSet_iter(tree, false), ^ void (ObjWrap item) {
        char *str = Obj_to_str(item);
        printf("%s\n", str);
    });

    AvlTreeSet_delete(&tree);

    printf("------------------------------------------------\n");

    tree = AvlTreeSet_new(&ordering_i32);
    int a = 1;
    int b = 3;
    int c = 6;
    int d = 4;
    int e = 2;
    int f = 7;
    if (AvlTreeSet_insert(tree, &a)) {
        printf("Inserted 1!\n");
    }
    if (AvlTreeSet_insert(tree, &b)) {
        printf("Inserted 3!\n");
    }
    if (AvlTreeSet_insert(tree, &c)) {
        printf("Inserted 6!\n");
    }
    if (AvlTreeSet_insert(tree, &d)) {
        printf("Inserted 4!\n");
    }
    if (AvlTreeSet_insert(tree, &e)) {
        printf("Inserted 2!\n");
    }
    if (AvlTreeSet_insert(tree, &f)) {
        printf("Inserted 7!\n");
    }

    Option_delete(AvlTreeSet_remove(tree, &a));
    Option_delete(AvlTreeSet_remove(tree, &b));

    AvlTreeSet_delete(&tree);
}


void vec_test_again() {
    Vec *vec = Vec_new();
    for (int i = 0; i < 1000000; i++) {
        char *tmp = malloc(20);
        strcpy(tmp, "Hello!");
        tmp[18] = '\0';
        
        Vec_push(vec, Obj_from_str(tmp));
    }

    Iter_for_each_obj(Vec_iter(vec, false), ^ void (ObjWrap item) {
        char *str = Obj_to_str(item);
        printf("%s\n", str);
    });

    Option_obj *goodbye = Vec_pop(vec);
    while (Option_obj_is_some(goodbye)) {
        free(Obj_to_str(Option_obj_get(goodbye)));
        goodbye = Vec_pop(vec);
    }
    Option_obj_delete(goodbye);
    Vec_delete(&vec);
}

void set_test() {
    AvlTreeSet *tree = AvlTreeSet_new(&ordering_str);
    Set set = AvlTreeSet_as_set(tree);

    char *hello = "hello!";
    set.vtable->push(set.set, hello);
    char *whats = "what's going on?";
    set.vtable->push(set.set, whats);
    char *hmm = "hmmm that's kinda odd";
    set.vtable->push(set.set, hmm);
    Iter_for_each_obj(set.vtable->iter(set.set, false), ^ void (ObjWrap item) {
        char *str = Obj_to_str(item);
        printf("%s\n", str);
    });
    set.vtable->delete(set.set);
}

void avl_tree_map_test() {
    AvlTreeMap *m = AvlTreeMap_new(&ordering_i32);
    AvlTreeMap_insert(m, Obj_from_i32(5), Obj_from_char('a'));
    AvlTreeMap_insert(m, Obj_from_i32(20), Obj_from_char('g'));
    AvlTreeMap_insert(m, Obj_from_i32(20), Obj_from_char('f'));
    AvlTreeMap_insert(m, Obj_from_i32(8), Obj_from_char('j'));
    Iter_for_each_obj(AvlTreeMap_iter(m, false), ^ void (ObjWrap item) {
        MapViewKV *view = Obj_to_ptr(item);
        printf("key: %d, val: %c\n", Obj_to_i32(view->key), Obj_to_char(view->value));
    });
    Option_obj *a_opt = AvlTreeMap_remove_obj(m, Obj_from_i32(20));
    if (Option_obj_is_some(a_opt)) {
        char g = Obj_to_char(Option_obj_get(a_opt));
        printf("%c\n", g);
    }

    Option_obj_delete(AvlTreeMap_remove_obj(m, Obj_from_i32(5)));
    Option_obj_delete(AvlTreeMap_remove_obj(m, Obj_from_i32(8)));
    if (AvlTreeMap_is_empty(m)) {
        printf("treemap is empty\n");
    }
    Iter_for_each_obj(AvlTreeMap_iter(m, false), ^ void (ObjWrap item) {
        MapViewKV *view = Obj_to_ptr(item);
        printf("key: %d, val: %c\n", Obj_to_i32(view->key), Obj_to_char(view->value));
    });

    AvlTreeMap_delete(&m);
}
