#ifdef __OPTION_TESTS
#include "../test/option.h"
#include "../test/munit.h"
#endif

#ifndef __OPTION_TESTS
#include "option/option_f32.h"
#endif
#include "option_template.h"

OPTION_TYPE_FACTORY(f32);
OPTION_TYPE_FUNCTION_FACTORY(f32);
OPTION_TYPE_MAP_FUNCTION_FACTORY(f32, i8);
OPTION_TYPE_MAP_FUNCTION_FACTORY(f32, i16);
OPTION_TYPE_MAP_FUNCTION_FACTORY(f32, i64);
OPTION_TYPE_MAP_FUNCTION_FACTORY(f32, u8);
OPTION_TYPE_MAP_FUNCTION_FACTORY(f32, u16);
OPTION_TYPE_MAP_FUNCTION_FACTORY(f32, u32);
OPTION_TYPE_MAP_FUNCTION_FACTORY(f32, u64);
OPTION_TYPE_MAP_FUNCTION_FACTORY(f32, float);
OPTION_TYPE_MAP_FUNCTION_FACTORY(f32, double);
OPTION_TYPE_MAP_FUNCTION_FACTORY(f32, size);

