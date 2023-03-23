#ifndef __INTERFACE_I32_H
#define __INTERFACE_I32_H

#include "interface/ordering.h"
#include <stdio.h>
#include <stdint.h>

static Ordering cmp_i32(const void *num1_ptr, const void *num2_ptr) {
    // printf("num1: %p\n", num1_ptr);
    // printf("num2: %p\n", num2_ptr);
    int32_t num1 = *(int32_t *) num1_ptr;
    int32_t num2 = *(int32_t *) num2_ptr;

    if (num1 < num2) {
        return Less;
    } else if (num1 > num2) {
        return Greater;
    } else {
        return Equal;
    }
}

static const OrderingVTable ordering_i32 = {
    .cmp = &cmp_i32,
};

#endif
