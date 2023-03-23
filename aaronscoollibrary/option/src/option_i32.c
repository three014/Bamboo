#ifdef __OPTION_TESTS
#include "../test/option.h"
#include "../test/munit.h"
#endif

#ifndef __OPTION_TESTS
#include "option.h"
#endif
#include "option_template.h"

OPTION_TYPE_FACTORY(i32);
OPTION_TYPE_FUNCTION_FACTORY(i32);



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
