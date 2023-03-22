#ifndef __INTERFACE_U32_H
#define __INTERFACE_U32_H

#include "interface/ordering.h"

Ordering cmp_u32(void *num1_ptr, void *num2_ptr) {
    unsigned int num1 = *(unsigned int *) num1_ptr;
    unsigned int num2 = *(unsigned int *) num2_ptr;

    if (num1 < num2) {
        return Less;
    } else if (num1 > num2) {
        return Greater;
    } else {
        return Equal;
    }
}

static const OrderingVTable ordering_u32 = {
    .cmp = &cmp_u32,
};

#endif
