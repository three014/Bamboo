#ifndef __JSON_H
#define __JSON_H

#include "result.h"

#include <stdint.h>

enum JsonErrorCode {
    TrailingComma,
    NoClosingToken,
    KeyWithoutValue
};

typedef struct __Json_Value_Struct JsonValue;

/**
 * Attempts to parse a JsonValue object from a 
 * string. 
 */
Result *Json_from_str(const char *str);

#endif // !__JSON_H
