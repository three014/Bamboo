#ifndef __MAP_H
#define __MAP_H

#include "interface/iterator.h"

#include <stddef.h>
#include <stdint.h>


typedef struct __Map_VTable_Struct
{
    IteratorVTable (*iter)(void *map, bool delete_collection_after_iter);
} MapVTable;

typedef struct __Map_Struct 
{
    const MapVTable *vtable;
    void *map;
} Map;


/* Key-Value Pair functions and objects */

typedef struct __Map_KeyValuePair_Struct {
    const void *key;
    const void *value;
} MapExternalKV;

typedef struct __MapEncoded_Repr_Struct {
    uint8_t hidden[18];
} MapEncodeKV;


MapEncodeKV MapKV_str_u16(const char *key, uint16_t value);
MapEncodeKV MapKV_size_u16(size_t key, uint16_t value);
MapEncodeKV MapKV_float_u16(float key, uint16_t value);
MapEncodeKV MapKV_double_u16(double key, uint16_t value);
MapEncodeKV MapKV_char_u16(char key, uint16_t value);
MapEncodeKV MapKV_u8_u16(uint8_t key, uint16_t value);
MapEncodeKV MapKV_u16_u16(uint16_t key, uint16_t value);
MapEncodeKV MapKV_u32_u16(uint32_t key, uint16_t value);
MapEncodeKV MapKV_u64_u16(uint64_t key, uint16_t value);
MapEncodeKV MapKV_i8_u16(int8_t key, uint16_t value);
MapEncodeKV MapKV_i16_u16(int16_t key, uint16_t value);
MapEncodeKV MapKV_i32_u16(int32_t key, uint16_t value);
MapEncodeKV MapKV_i64_u16(int64_t key, uint16_t value);

MapEncodeKV MapKV_str_u32(const char *key, uint32_t value);
MapEncodeKV MapKV_size_u32(size_t key, uint32_t value);
MapEncodeKV MapKV_float_u32(float key, uint32_t value);
MapEncodeKV MapKV_double_u32(double key, uint32_t value);
MapEncodeKV MapKV_char_u32(char key, uint32_t value);
MapEncodeKV MapKV_u8_u32(uint8_t key, uint32_t value);
MapEncodeKV MapKV_u16_u32(uint16_t key, uint32_t value);
MapEncodeKV MapKV_u32_u32(uint32_t key, uint32_t value);
MapEncodeKV MapKV_u64_u32(uint64_t key, uint32_t value);
MapEncodeKV MapKV_i8_u32(int8_t key, uint32_t value);
MapEncodeKV MapKV_i16_u32(int16_t key, uint32_t value);
MapEncodeKV MapKV_i32_u32(int32_t key, uint32_t value);
MapEncodeKV MapKV_i64_u32(int64_t key, uint32_t value);

MapEncodeKV MapKV_str_u64(const char *key, uint64_t value);
MapEncodeKV MapKV_size_u64(size_t key, uint64_t value);
MapEncodeKV MapKV_float_u64(float key, uint64_t value);
MapEncodeKV MapKV_double_u64(double key, uint64_t value);
MapEncodeKV MapKV_char_u64(char key, uint64_t value);
MapEncodeKV MapKV_u8_u64(uint8_t key, uint64_t value);
MapEncodeKV MapKV_u16_u64(uint16_t key, uint64_t value);
MapEncodeKV MapKV_u32_u64(uint32_t key, uint64_t value);
MapEncodeKV MapKV_u64_u64(uint64_t key, uint64_t value);
MapEncodeKV MapKV_i8_u64(int8_t key, uint64_t value);
MapEncodeKV MapKV_i16_u64(int16_t key, uint64_t value);
MapEncodeKV MapKV_i32_u64(int32_t key, uint64_t value);
MapEncodeKV MapKV_i64_u64(int64_t key, uint64_t value);

MapEncodeKV MapKV_str_i8(const char *key, int8_t value);
MapEncodeKV MapKV_size_i8(size_t key, int8_t value);
MapEncodeKV MapKV_float_i8(float key, int8_t value);
MapEncodeKV MapKV_double_i8(double key, int8_t value);
MapEncodeKV MapKV_char_i8(char key, int8_t value);
MapEncodeKV MapKV_u8_i8(uint8_t key, int8_t value);
MapEncodeKV MapKV_u16_i8(uint16_t key, int8_t value);
MapEncodeKV MapKV_u32_i8(uint32_t key, int8_t value);
MapEncodeKV MapKV_u64_i8(uint64_t key, int8_t value);
MapEncodeKV MapKV_i8_i8(int8_t key, int8_t value);
MapEncodeKV MapKV_i16_i8(int16_t key, int8_t value);
MapEncodeKV MapKV_i32_i8(int32_t key, int8_t value);
MapEncodeKV MapKV_i64_i8(int64_t key, int8_t value);

MapEncodeKV MapKV_str_i16(const char *key, int16_t value);
MapEncodeKV MapKV_size_i16(size_t key, int16_t value);
MapEncodeKV MapKV_float_i16(float key, int16_t value);
MapEncodeKV MapKV_double_i16(double key, int16_t value);
MapEncodeKV MapKV_char_i16(char key, int16_t value);
MapEncodeKV MapKV_u8_i16(uint8_t key, int16_t value);
MapEncodeKV MapKV_u16_i16(uint16_t key, int16_t value);
MapEncodeKV MapKV_u32_i16(uint32_t key, int16_t value);
MapEncodeKV MapKV_u64_i16(uint64_t key, int16_t value);
MapEncodeKV MapKV_i8_i16(int8_t key, int16_t value);
MapEncodeKV MapKV_i16_i16(int16_t key, int16_t value);
MapEncodeKV MapKV_i32_i16(int32_t key, int16_t value);
MapEncodeKV MapKV_i64_i16(int64_t key, int16_t value);

MapEncodeKV MapKV_str_i32(const char *key, int32_t value);
MapEncodeKV MapKV_size_i32(size_t key, int32_t value);
MapEncodeKV MapKV_float_i32(float key, int32_t value);
MapEncodeKV MapKV_double_i32(double key, int32_t value);
MapEncodeKV MapKV_char_i32(char key, int32_t value);
MapEncodeKV MapKV_u8_i32(uint8_t key, int32_t value);
MapEncodeKV MapKV_u16_i32(uint16_t key, int32_t value);
MapEncodeKV MapKV_u32_i32(uint32_t key, int32_t value);
MapEncodeKV MapKV_u64_i32(uint64_t key, int32_t value);
MapEncodeKV MapKV_i8_i32(int8_t key, int32_t value);
MapEncodeKV MapKV_i16_i32(int16_t key, int32_t value);
MapEncodeKV MapKV_i32_i32(int32_t key, int32_t value);
MapEncodeKV MapKV_i64_i32(int64_t key, int32_t value);

MapEncodeKV MapKV_str_i64(const char *key, int64_t value);
MapEncodeKV MapKV_size_i64(size_t key, int64_t value);
MapEncodeKV MapKV_float_i64(float key, int64_t value);
MapEncodeKV MapKV_double_i64(double key, int64_t value);
MapEncodeKV MapKV_char_i64(char key, int64_t value);
MapEncodeKV MapKV_u8_i64(uint8_t key, int64_t value);
MapEncodeKV MapKV_u16_i64(uint16_t key, int64_t value);
MapEncodeKV MapKV_u32_i64(uint32_t key, int64_t value);
MapEncodeKV MapKV_u64_i64(uint64_t key, int64_t value);
MapEncodeKV MapKV_i8_i64(int8_t key, int64_t value);
MapEncodeKV MapKV_i16_i64(int16_t key, int64_t value);
MapEncodeKV MapKV_i32_i64(int32_t key, int64_t value);
MapEncodeKV MapKV_i64_i64(int64_t key, int64_t value);


#endif // !__MAP_H
#define __MAP_H
