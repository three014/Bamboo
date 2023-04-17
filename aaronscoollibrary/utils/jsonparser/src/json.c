#include "json_p.h"


Result *Json_from_str(const char *str) {
    Result *ret = NULL;
    char buffer[256] = {0};


    return ret;
}



JsonValue *JsonValue_new(enum JsonType type, ObjWrap value) {
    JsonValue *ret = malloc(sizeof *ret);
    ret->type = type;
    ret->v = ObjWrap_to_internal(value);
    return ret;
}

JsonValue *JsonValue_null() {
    JsonValue *ret = malloc(sizeof *ret);
    ret->type = NullVal;
    return ret;
}

bool JsonValue_replace_null(JsonValue *json, enum JsonType type, ObjWrap value) {
    bool replaced = false;
    if (json->type == NullVal) {
        json->type = type;
        json->v = ObjWrap_to_internal(value);
        replaced = true;
    }
    return replaced;
}

void JsonValue_delete_recur(JsonValue *json_value) {
    switch (json_value->type) {
        case StrVal:
            free(Obj_to_str(ObjectInternal_to_wrap(json_value->v)));
            /* fallthrough */
        case NullVal:
            /* fallthrough */
        case BoolVal:
            /* fallthrough */
        case NumVal:
            break;
        case ArrVal: {
            Vec *v = Obj_to_ptr(ObjectInternal_to_wrap(json_value->v));
            Vec_deep_delete(v, ^ void (void *value) {
                JsonValue_delete_recur(value);
            });
            break;
        }
        case ObjVal: {
            Map_StrVal *m = Obj_to_ptr(ObjectInternal_to_wrap(json_value->v));
            m->vtable->deep_delete(
                m->map,
                ^ void(void *key) {
                    free(key);
                }, ^ void(void *value) {
                    JsonValue_delete_recur(value);
                }
            );
            free(m);
            break;
        }
    }
    JsonValue_delete(json_value);
}


void JsonValue_delete(JsonValue *json_value) {
    free(json_value);
}

