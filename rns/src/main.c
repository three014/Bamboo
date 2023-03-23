#include <math.h>
#include <stdio.h>
#include "avltreeset.h"
#include "interface/interface_str.h"
#include "interface/iterator.h"
#include "main.h"
#include "vector.h"

void vec_test();
void string_test();
void avltree_test();
void vec_test_again();

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
    Vec_push(str_vec, e);
    Vec_push(str_vec, s);
    Vec_push(str_vec, g);

    for_each(Vec_iter(str_vec), ^ void (void *item) {
        char *str = item;
        printf("char *str = %p\n", str);
        printf("%s\n", str);

    });
    
    Vec_delete(str_vec);
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

void vec_test() {
    
    Test *t = malloc(sizeof *t);
    t->x = 4;
    t->y = 'c';
    t->z = 3.4;
    Test *t1 = malloc(sizeof *t1);
    t1->x = 3;
    t1->y = 'c';
    t1->z = 6.432;
    Test *t2 = malloc(sizeof *t2);
    t2->x = 6;
    t2->y = 's';
    t2->z = 46.2;
    Test *t3 = malloc(sizeof *t3);
    t3->x = 5;
    t3->y = 'g';
    t3->z = 3.67;

    Vec *test_vec = Vec_with_capacity(2);
    Vec_push(test_vec, t);
    Vec_push(test_vec, t1);
    Vec_push(test_vec, t2);
    Vec_push(test_vec, t3);

    IteratorVTable *mapped_vec_iter = map_iter(Vec_iter(test_vec), ^ void *(void *item) {
        char *str = malloc(64);
        // char str[64];
        Test *test = item;
        sprintf(str, "Test: x = %d, y = %c, z = %lf\n", test->x, test->y, test->z);
        str[63] = '\0';
        return str;
    });
   
    for_each(mapped_vec_iter, ^ void (void *item) {
        char *str = item;
        printf("%s\n", str);
        free(str);
    });

    OrderingVTable test_ord = {
        .cmp = cmp_test,
    };

    Vec *vec2 = collect(Vec_iter(test_vec), Vec_constr());
    AvlTreeSet *tree = collect(Vec_iter(test_vec), AvlTreeSet_constr(&test_ord));

    
    AvlTreeSet_delete(&tree);
    Vec_delete(vec2);
    Vec_delete(test_vec);

    free(t);
    free(t1);
    free(t2);
    free(t3);

    Vec_u32 *v = Vec_u32_new();
    Vec_u32_push(v, 1);
    Vec_u32_delete(v);
}

void avltree_test() {
    AvlTreeSet *tree = AvlTreeSet_with_ordering(&ordering_str);
    char *x = "Today is a good day for to have to have a good day";
    char *y = "Tomorrow is a gift";
    char *z = "That's why yesterday is a yesterday";
    AvlTreeSet_insert(tree, y);
    AvlTreeSet_insert(tree, x);
    AvlTreeSet_insert(tree, z);

    for_each(AvlTreeSet_iter(tree), ^ void (void *item) {
        char *str = item;
        printf("%s\n", str);
    });

    AvlTreeSet_delete(&tree);

    tree = AvlTreeSet_with_ordering(&ordering_i32);
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

    // AvlTreeSet_remove(tree, &a);

    for_each_enumerate(AvlTreeSet_iter(tree), ^ void (const unsigned long index, void *item) {
        printf("%lu, %d\n", index, *(int32_t *) item);
    });

    AvlTreeSet_delete(&tree);
}


void vec_test_again() {
    Vec *vec = Vec_new();
    for (int i = 0; i < 1000000; i++) {
        char *tmp = malloc(20);
        strcpy(tmp, "Hello!");
        tmp[18] = '\0';
        
        Vec_push(vec, tmp);
    }

    for_each(Vec_iter(vec), ^ void (void *item) {
        char *str = item;
        printf("%s\n", str);
    });

    Option *goodbye = Vec_pop(vec);
    while (Option_is_some(goodbye)) {
        free(Option_get(goodbye));
        goodbye = Vec_pop(vec);
    }
    Option_delete(goodbye);
    Vec_delete(vec);

}
