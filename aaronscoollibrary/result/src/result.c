#include "result.h"

#include <stdlib.h>

typedef struct __Result_Struct {
    ObjWrap item;
    bool is_ok;
} Result;

Result *Result_ok(ObjWrap ok_item) {
    Result *result = malloc(sizeof *result);
    result->item = ok_item;
    result->is_ok = true;
    return result;
}

Result *Result_err(ObjWrap err_item) {
    Result *result = malloc(sizeof *result);
    result->item = err_item;
    result->is_ok = false;
    return result;
}

bool Result_is_ok(Result *self) {
    return self->is_ok;
}

bool Result_is_err(Result *self) {
    return !self->is_ok;
}

ObjWrap Result_get(Result *self) {
    ObjWrap ret = self->item;
    Result_delete(self);
    return ret;
}

void Result_delete(Result *self) {
    free(self);
}

