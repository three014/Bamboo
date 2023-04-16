#ifndef __RESULT_H
#define __RESULT_H

#include "object.h"

#include <stdbool.h>

typedef struct __Result_Struct Result;

Result  *Result_ok(ObjWrap ok_item);
Result  *Result_err(ObjWrap err_item);
bool    Result_is_ok(Result *self);
bool    Result_is_err(Result *self);
ObjWrap Result_get(Result *self);
void    Result_delete(Result *self);

#endif // !__RESULT_H
