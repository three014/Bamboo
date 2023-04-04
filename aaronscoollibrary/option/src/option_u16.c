#ifdef __OPTION_TESTS
#include "../test/option.h"
#include "../test/munit.h"
#endif

#ifndef __OPTION_TESTS
#include "option/option_u16.h"
#endif
#include "option_template.h"

OPTION_TYPE_FACTORY(u16);
OPTION_TYPE_FUNCTION_FACTORY(u16);
OPTION_TYPE_MAP_FUNCTION_FACTORY(u16, i8);
OPTION_TYPE_MAP_FUNCTION_FACTORY(u16, i16);
OPTION_TYPE_MAP_FUNCTION_FACTORY(u16, i64);
OPTION_TYPE_MAP_FUNCTION_FACTORY(u16, u8);
OPTION_TYPE_MAP_FUNCTION_FACTORY(u16, u16);
OPTION_TYPE_MAP_FUNCTION_FACTORY(u16, u32);
OPTION_TYPE_MAP_FUNCTION_FACTORY(u16, u64);
OPTION_TYPE_MAP_FUNCTION_FACTORY(u16, float);
OPTION_TYPE_MAP_FUNCTION_FACTORY(u16, double);
OPTION_TYPE_MAP_FUNCTION_FACTORY(u16, size);

