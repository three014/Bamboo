#ifndef __JSON_H
#define __JSON_H

#include "option/option_obj.h"
#include "result.h"

#include <stdint.h>

typedef struct __Json_Value_To_Object_Template JsonTemplate;
typedef struct __Json_Value_Struct JsonValue;

enum JsonParseError {
    IoError,
    BadSyntax,
    Incomplete
};

/**
 * Attempts to parse a JsonValue object from a 
 * null-terminated char string.
 *
 * Returns a Result with the format:
 *
 * Result<ObjWrap<JsonValue *>, ObjWrap<enum JsonParseError>>
 *
 * On success, the unwrapped JsonValue obj must be consumed
 * by either deleting the object or converting it to 
 * a data structure with 
 *
 * Json_to_type(JsonValue *json, JsonTemplate *template);
 */
Result *Json_from_str(const char *str);


/**
 * Attempts to convert a JsonValue object to
 * a Vector. Requires a JsonTemplate to deserialize
 * the values inside of the vector. Also requires
 * the JsonValue array to be homogenous.
 *
 * Does not alter the JsonTemplate or JsonValue.
 *
 * Returns a Result with the format:
 *
 * Result<ObjWrap<Vec<T> *>, ObjWrap<enum JsonDeserializeError>>
 * where T: char * | int32_t | bool | [custom_struct_type] * | Vec * | Map *
 */
Result *Json_to_vec(const JsonValue *json, const JsonTemplate *value_template);

/**
 * Attempts to convert a JsonValue object to
 * a Map. Requires a JsonTemplate to deserialize
 * the values inside of the map. Also requires 
 * the JsonValue map to be homogenous.
 *
 * On success, the unwrapped ObjWrap is a pointer to
 * a Map object. However, Map objects are just
 * wrappers to the map's vtable and actual map data structure.
 * Therefore, the user can dereference the map pointer and
 * make a copy of the struct, then free the map pointer.
 * Upon map deletion, the user must use Map_deep_delete
 * to free the keys, since all the map's string keys
 * were heap allocated.
 *
 * Does not alter the JsonTemplate or JsonValue.
 *
 * Returns a Result with the format:
 *
 * Result<ObjWrap<Map<char *, V>, ObjWrap<enum JsonDeserializeError>>
 * where V: char * | int32_t | bool | [custom_struct_type] * | Vec * | Map *
 */
Result *Json_to_map(const JsonValue *json, const JsonTemplate *value_template);

/**
 * Attempts to convert a JsonValue object to
 * a custom struct. Requires a JsonTemplate to describe 
 * how to create the struct from the JsonValue object's
 * key/value pairs. 
 *
 * If the JsonValue 
 */
Result *Json_to_struct(const JsonValue *json, const JsonTemplate *struct_template);


JsonTemplate *JT_str();
JsonTemplate *JT_num();
JsonTemplate *JT_bool();
JsonTemplate *JT_vec(JsonTemplate *value_template);
JsonTemplate *JT_map(JsonTemplate *value_template);
JsonTemplate *JT_struct(JsonTemplate *field_template, ...);

/**
 * Special-case JsonTemplate that accepts a string for a key along with
 * a JsonTemplate for the value.
 *
 * Can only be used in the struct template, JT_struct(). Passing in a 
 * template created with this function into the vec or map template functions
 * results in a null pointer, and will not free the passed-in template.
 */
JsonTemplate *JT_kv(const char *key, JsonTemplate *value_template);

/**
 * Deletes and frees the memory for a JsonTemplate.
 */
void JT_delete(JsonTemplate *json_template);

/**
 * Deletes and frees the memory for a JsonTemplate,
 * but also recursively frees the memory for JsonTemplates 
 * that were associated with the passed-in template.
 */
void JT_delete_r(JsonTemplate *json_template);



/**
 * Accesses a JsonValue at the specified field name. 
 *
 * If the JsonValue is another object or array, then the
 * ObjWrap will be a pointer to the JsonValue containing 
 * that data. 
 *
 * If the JsonValue is a string, number, or boolean,
 * then the ObjWrap will be a char pointer, int32_t,
 * or bool, respectively.
 *
 * If the JsonValue is null, the field name doesn't exist,
 * of if the parent JsonValue isn't an obj type,
 * then the ObjWrap will be an enum JsonAccessError.
 *
 * Format of returned item:
 *
 * Result<ObjWrap<T>, ObjWrap<enum JsonAccessError>>,
 *     where T: char * | int32_t | bool | JsonValue *
 */
Result *Json_obj_view(JsonValue *json, const char *field_name);

/**
 * Accesses a JsonValue at the specified index. 
 *
 * If the accessed JsonValue is another object or array, then the
 * ObjWrap will be a pointer to the JsonValue containing 
 * that data. 
 *
 * If the accessed JsonValue is a string, number, or boolean,
 * then the ObjWrap will be a char pointer, int32_t,
 * or bool, respectively.
 *
 * If the accessed JsonValue is null, the index is out-of-bounds,
 * or if the parent JsonValue isn't an array type,
 * then the ObjWrap will be an enum JsonAccessError.
 *
 * Format of returned item:
 *
 * Result<ObjWrap<T>, ObjWrap<enum JsonAccessError>>,
 *     where T: char * | int32_t | bool | JsonValue *
 */
Result *Json_arr_view(JsonValue *json, size_t index);



Result *Json_view(JsonValue *json, const char *pointer);






#endif // !__JSON_H
