#ifndef __INTERFACE_STR_H
#define __INTERFACE_STR_H

#include "interface/ordering.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

static Ordering cmp_str(const void *str1, const void *str2) {
    // printf("str1: %p ---- str2: %p\n", str1, str2);
    char buf1[16];
    char buf2[16];
    strncpy(buf1, str1, 15);
    buf1[15] = '\0';
    strncpy(buf2, str2, 15);
    buf2[15] = '\0';
    int32_t cmp = strncmp(buf1, buf2, 16);
    if (cmp > 0) {
        return Greater;
    }
    else if (cmp < 0)
    {
        return Less;
    }
    return Equal;
}

static const OrderingVTable ordering_str = {
    .cmp = &cmp_str,
};



#endif // !__INTERFACE_STR
