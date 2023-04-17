#ifndef __JSON_P_H
#define __JSON_P_H

#include "interface/map.h"
#include "interface/iterator.h"
#include "object.h"
#include "option/option_obj.h"
#include "utils/json.h"
#include "vector/vector.h"

#include "../../../interface/src/object/object_p.h"

#include <string.h>

#define BUFFER_SIZE 256
#define OPEN_BRACE ('{')
#define CLOSE_BRACE ('}')
#define OPEN_BRACKET ('[')
#define CLOSE_BRACKET (']')
#define COLON (':')


typedef Map Map_StrVal;


struct __Json_Value_Struct {
    ObjectInternal v;
    enum JsonType {

        // Null Format: (void *) 0
        NullVal,

        // Bool Format: True or False
        BoolVal,

        // Num Format: int32_t
        NumVal,

        // Str Format: char * (size: 256)
        StrVal,

        // Arr Format: Vec<JsonValue *> *
        ArrVal,

        // Obj Format: Map<char *, JsonValue *> *
        ObjVal
    } type;
};


enum JsonParseInnerStateMachine {
    ExpectCurlyOrBracketOrQuoteOrValue,
    ExpectStartQuoteForKey,
    ExpectColonForValue,
    ExpectTextOrEndQuote,
    ExpectNumOrCommaOrWhitespace,
    ExpectCommaOrEndCurlyOrEndBracket,
};

enum JsonParseStateMachine {
    Object,
    AnyIdentifier,
    NumBoolStrNull,
    Array,
    ErrorAbort,
    EndParse
};


void        JsonValue_delete(JsonValue *json_value);
void        JsonValue_delete_recur(JsonValue *json_value);
JsonValue   *JsonValue_new(enum JsonType type, ObjWrap value);
JsonValue   *JsonValue_null();
bool        JsonValue_replace_null(JsonValue *json, enum JsonType type, ObjWrap value);

#endif // !__JSON_P_H
