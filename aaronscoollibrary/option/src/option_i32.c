#ifdef __OPTION_TESTS
#include "../test/option.h"
#include "../test/munit.h"
#endif

#ifndef __OPTION_TESTS
#include "option/option_i32.h"
#endif
#include "option_template.h"

OPTION_TYPE_FACTORY(i32);
OPTION_TYPE_FUNCTION_FACTORY(i32);
OPTION_TYPE_MAP_FUNCTION_FACTORY(i32, i8);
OPTION_TYPE_MAP_FUNCTION_FACTORY(i32, i16);
OPTION_TYPE_MAP_FUNCTION_FACTORY(i32, i64);
OPTION_TYPE_MAP_FUNCTION_FACTORY(i32, u8);
OPTION_TYPE_MAP_FUNCTION_FACTORY(i32, u16);
OPTION_TYPE_MAP_FUNCTION_FACTORY(i32, u32);
OPTION_TYPE_MAP_FUNCTION_FACTORY(i32, u64);
OPTION_TYPE_MAP_FUNCTION_FACTORY(i32, float);
OPTION_TYPE_MAP_FUNCTION_FACTORY(i32, double);
OPTION_TYPE_MAP_FUNCTION_FACTORY(i32, size);

#ifdef __OPTION_TESTS
static MunitResult 
option_i32_get_test(const MunitParameter params[], void *fixture)
{
    int32_t rand = munit_rand_int_range(INT32_MIN, INT32_MAX);
    Option_i32 *option = Option_i32_of(rand);
    munit_assert_int(Option_i32_get(option), ==, rand);
    return MUNIT_OK;
}

static MunitResult
option_i32_clone_test(const MunitParameter params[], void *fixture)
{
    int32_t rand = munit_rand_int_range(INT32_MIN, INT32_MAX);
    Option_i32 *option = Option_i32_of(rand);
    Option_i32 *option_clone = Option_i32_clone(option);
    munit_assert_memory_equal(sizeof(Option_i32), option, option_clone);
    Option_i32_delete(option);
    Option_i32_delete(option_clone);
    return MUNIT_OK;
}

static MunitResult
option_i32_take_test(const MunitParameter params[], void *fixture)
{
    int32_t rand = munit_rand_int_range(INT32_MIN, INT32_MAX);
    Option_i32 *option = Option_i32_of(rand);
    Option_i32_delete(Option_i32_take(option));
    munit_assert_false(Option_i32_is_some(option));
    Option_i32_delete(option);
    return MUNIT_OK;
}

MunitTest tests[] = 
{
    {
        "/option_i32_get",
        option_i32_get_test,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL,
    },
    {
        "/option_i32_clone",
        option_i32_clone_test,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL,
    },
    {
        "/option_i32_take",
        option_i32_take_test,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL,
    },
    { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
};

static const MunitSuite suite =
{
    "/my-tests",
    tests,
    NULL,
    1,
    MUNIT_SUITE_OPTION_NONE,
};

int main(int argc, char **argv) 
{
    return munit_suite_main(&suite, NULL, argc, argv);
}

#endif
