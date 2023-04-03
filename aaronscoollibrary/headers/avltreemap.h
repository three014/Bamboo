#ifndef __AVLTREEMAP_H
#define __AVLTREEMAP_H

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "option/option.h"
#include "interface/ordering.h"


// Goal: Use any type as the key, and any pointer type as the value
// Idea #1: Create Key Struct (which will just wrap a uint64_t) 
//          and make the user hash it into a uint64_t?
// Idea #2: Use void pointers again, and pass in addr of key 
//          (requires key to be initialized right before func call if on stack,
//          and memory management to occur if heap-allocated)
//          Along the same vein: create wrapper for primitives, 
//          but then this requires a way to free the object inside the function
//          call? Which kind of breaks a C idiom 
//          
//          We could use the wrappers to create keys of the primitives, which
//          then get stored in the map and free'd upon map deletion,
//          and then only use addresses of stack allocated primitives to 
//          compare with the internal keys? (Should be okay, since the stack
//          alloc'd keys should be temporary and should exist for the map
//          function calls)
//
//          Still not very clean...
// Idea #3: Get crazy with the macros? Issue is that I don't 
//          have the skill or knowledge to do that.
//
//          Crazy sed project? Create template H and C file, 
//          use some keyword in place of the type, then use 
//          sed to replace that type with the correct type 
//          for each primitive type
// Idea #4: Use an 8-byte field that can store either a 1. Memory address to some type of 
//          large key, or 2. Some primitive value like an int or char, stored on the lowest
//          bytes of this field.
//
//          If pointer, then cast the (size_t probably) to an object pointer then go crazy
//          If primitive value, then cast to the primitive value and it (probably) won't 
//          mess up the value. - Not perfect, but should work for here.
//
//          How to standardize it?

// Working with Idea #4: 
// - We could have a Key Struct that has maybe a size_t for the key,
//   and a bool for 
//   - is this a primitive stack-alloc'd value? Or
//   - is this a pointer to some other value (like a string)?
//     - we can't assume that the value was malloc'd, so no free()
//     - user should free anything they create
// By the time the key enters the compare function, it should be a (void *)
// - For malloc'd items, just cast as a (void *) so the user can cast as other type pointer
// - For stack alloc'd items, pass the addr of the key so user can cast as other type and deref
//   - Have to worry about byte ordering, because, for example,
//     - If value was int, stored in this size_t, then casting it as int could chop off the part that
//       was actually the int value
//     - At least I think that might be an issue? We'll have to check
// When the user enters a key, they should have a nice way to enter the function:
// - bool AvlTreeMap_insert(AvlTreeMap *self, MapInternalKV kv);
// ISSUE: Compiler complains that MapInternalKV doesn't have a known size
// FIX: Use a struct that contains an array of bytes, use function to convert key/value to
//      byte representation, then pass this array into map data structure, where it'll convert 
//      it back into the internal key/value structure
//   POSSIBLE ISSUE: Byte ordering - Right now it works, but what if the byte ordering was reversed? Would it
//                   still work? How could I test that?

typedef struct __AvlTreeMap_Struct AvlTreeMap;

typedef struct __Map_KeyValuePair_Struct {
    const void *key;
    const void *value;
} MapExternalKV;

typedef struct __MapExternal_Repr_Struct {
    uint8_t hidden[18];
} MapKV;

MapKV MapKV_str_u32(const char *key, uint32_t value);
MapKV MapKV_size_u32(size_t key, uint32_t value);
MapKV MapKV_float_u32(float key, uint32_t value);
MapKV MapKV_double_u32(double key, uint32_t value);
MapKV MapKV_char_u32(char key, uint32_t value);
MapKV MapKV_u8_u32(uint8_t key, uint32_t value);
MapKV MapKV_u16_u32(uint16_t key, uint32_t value);
MapKV MapKV_u32_u32(uint32_t key, uint32_t value);
MapKV MapKV_u64_u32(uint64_t key, uint32_t value);
MapKV MapKV_i8_u32(int8_t key, uint32_t value);
MapKV MapKV_i16_u32(int16_t key, uint32_t value);
MapKV MapKV_i32_u32(int32_t key, uint32_t value);
MapKV MapKV_i64_u32(int64_t key, uint32_t value);
MapKV MapKV_str_u64(const char *key, uint64_t value);
MapKV MapKV_size_u64(size_t key, uint64_t value);
MapKV MapKV_float_u64(float key, uint64_t value);
MapKV MapKV_double_u64(double key, uint64_t value);
MapKV MapKV_char_u64(char key, uint64_t value);
MapKV MapKV_u8_u64(uint8_t key, uint64_t value);
MapKV MapKV_u16_u64(uint16_t key, uint64_t value);
MapKV MapKV_u32_u64(uint32_t key, uint64_t value);
MapKV MapKV_u64_u64(uint64_t key, uint64_t value);
MapKV MapKV_i8_u64(int8_t key, uint64_t value);
MapKV MapKV_i16_u64(int16_t key, uint64_t value);
MapKV MapKV_i32_u64(int32_t key, uint64_t value);
MapKV MapKV_i64_u64(int64_t key, uint64_t value);

MapKV MapKV_str_i8(const char *key, int8_t value);
MapKV MapKV_size_i8(size_t key, int8_t value);
MapKV MapKV_float_i8(float key, int8_t value);
MapKV MapKV_double_i8(double key, int8_t value);
MapKV MapKV_char_i8(char key, int8_t value);
MapKV MapKV_u8_i8(uint8_t key, int8_t value);
MapKV MapKV_u16_i8(uint16_t key, int8_t value);
MapKV MapKV_u32_i8(uint32_t key, int8_t value);
MapKV MapKV_u64_i8(uint64_t key, int8_t value);
MapKV MapKV_i8_i8(int8_t key, int8_t value);
MapKV MapKV_i16_i8(int16_t key, int8_t value);
MapKV MapKV_i32_i8(int32_t key, int8_t value);
MapKV MapKV_i64_i8(int64_t key, int8_t value);

MapKV MapKV_str_i16(const char *key, int16_t value);
MapKV MapKV_size_i16(size_t key, int16_t value);
MapKV MapKV_float_i16(float key, int16_t value);
MapKV MapKV_double_i16(double key, int16_t value);
MapKV MapKV_char_i16(char key, int16_t value);
MapKV MapKV_u8_i16(uint8_t key, int16_t value);
MapKV MapKV_u16_i16(uint16_t key, int16_t value);
MapKV MapKV_u32_i16(uint32_t key, int16_t value);
MapKV MapKV_u64_i16(uint64_t key, int16_t value);
MapKV MapKV_i8_i16(int8_t key, int16_t value);
MapKV MapKV_i16_i16(int16_t key, int16_t value);
MapKV MapKV_i32_i16(int32_t key, int16_t value);
MapKV MapKV_i64_i16(int64_t key, int16_t value);

MapKV MapKV_str_i32(const char *key, int32_t value);
MapKV MapKV_size_i32(size_t key, int32_t value);
MapKV MapKV_float_i32(float key, int32_t value);
MapKV MapKV_double_i32(double key, int32_t value);
MapKV MapKV_char_i32(char key, int32_t value);
MapKV MapKV_u8_i32(uint8_t key, int32_t value);
MapKV MapKV_u16_i32(uint16_t key, int32_t value);
MapKV MapKV_u32_i32(uint32_t key, int32_t value);
MapKV MapKV_u64_i32(uint64_t key, int32_t value);
MapKV MapKV_i8_i32(int8_t key, int32_t value);
MapKV MapKV_i16_i32(int16_t key, int32_t value);
MapKV MapKV_i32_i32(int32_t key, int32_t value);
MapKV MapKV_i64_i32(int64_t key, int32_t value);

MapKV MapKV_str_i64(const char *key, int64_t value);
MapKV MapKV_size_i64(size_t key, int64_t value);
MapKV MapKV_float_i64(float key, int64_t value);
MapKV MapKV_double_i64(double key, int64_t value);
MapKV MapKV_char_i64(char key, int64_t value);
MapKV MapKV_u8_i64(uint8_t key, int64_t value);
MapKV MapKV_u16_i64(uint16_t key, int64_t value);
MapKV MapKV_u32_i64(uint32_t key, int64_t value);
MapKV MapKV_u64_i64(uint64_t key, int64_t value);
MapKV MapKV_i8_i64(int8_t key, int64_t value);
MapKV MapKV_i16_i64(int16_t key, int64_t value);
MapKV MapKV_i32_i64(int32_t key, int64_t value);
MapKV MapKV_i64_i64(int64_t key, int64_t value);




#endif // !__AVLTREEMAP_H
