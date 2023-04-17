#include "interface/iterator.h"
#include "interface/map.h"
#include "object.h"
#include "../../../interface/src/object/object_p.h"
#include "option/option_obj.h"
#include "utils/json.h"
#include "vector/vector.h"
#include <string.h>

#define BUFFER_SIZE 256
#define OPEN_BRACE ('{')
#define CLOSE_BRACE ('}')
#define OPEN_BRACKET ('[')
#define CLOSE_BRACKET (']')
#define COLON (':')


typedef const char *JsonKey;

struct __Json_Value_Struct {
    ObjectInternal v;
    enum JsonType {
        NullVal,
        BoolVal,
        NumVal,
        StrVal,
        ArrVal,
        ObjVal
    } type;
};


void Json_delete_obj(JsonValue *json_value);
void Json_delete_structure(JsonValue *json_value);

void Json_delete_structure(JsonValue *json_value) {
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
                Json_delete_structure(value);
            });
            break;
        }
        case ObjVal: {
            Map *m = Obj_to_ptr(ObjectInternal_to_wrap(json_value->v));
            m->vtable->deep_delete(
                m->map,
                ^ void(void *key) {
                    free(key);
                }, ^ void(void *value) {
                    Json_delete_structure(value);
                }
            );
            break;
        }
    }
    Json_delete_obj(json_value);
}


void Json_delete_obj(JsonValue *json_value) {
    free(json_value);
}

