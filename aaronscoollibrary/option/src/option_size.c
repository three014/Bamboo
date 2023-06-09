#ifdef __OPTION_TESTS
#include "../test/option.h"
#include "../test/munit.h"
#endif

#ifndef __OPTION_TESTS
#include "option/option_size.h"
#endif
#include "option_template.h"

OPTION_TYPE_FACTORY(size);
OPTION_TYPE_FUNCTION_FACTORY(size);
OPTION_TYPE_MAP_FUNCTION_FACTORY(size, i8);
OPTION_TYPE_MAP_FUNCTION_FACTORY(size, i16);
OPTION_TYPE_MAP_FUNCTION_FACTORY(size, i64);
OPTION_TYPE_MAP_FUNCTION_FACTORY(size, u8);
OPTION_TYPE_MAP_FUNCTION_FACTORY(size, u16);
OPTION_TYPE_MAP_FUNCTION_FACTORY(size, u32);
OPTION_TYPE_MAP_FUNCTION_FACTORY(size, u64);
OPTION_TYPE_MAP_FUNCTION_FACTORY(size, float);
OPTION_TYPE_MAP_FUNCTION_FACTORY(size, double);
OPTION_TYPE_MAP_FUNCTION_FACTORY(size, size);

