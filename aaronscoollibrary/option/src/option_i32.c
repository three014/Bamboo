#include "option.h"

struct __Option_i32_Struct
{
    bool some;
    int item;
};

Option_i32 *Option_i32_of(void *item)
{
    Option_i32 *ret = malloc(sizeof *ret);
    if (item == NULL)
    {
        ret->item = 0;
        ret->some = false;
    }
    else 
    {
        ret->item = *(int *) item;
        ret->some = true;
    }
    return ret;
}

void Option_i32_delete(Option_i32 *option)
{
    free(option);
}

bool Option_i32_is_some(Option_i32 *option)
{
    if (option == NULL)
    {
        return false;
    }
    return option->some;
}

bool Option_i32_is_some_and(Option_i32 *option, bool (^predicate)(int))
{
    if (option == NULL || 
            Option_i32_is_some(option) == false ||
            predicate == NULL)
    {
        return false;
    }
    return predicate(option->item);
}

int Option_i32_get(Option_i32 *option)
{
    int ret = option->item;
    Option_i32_delete(option);
    return ret;
}

int Option_i32_get_or(Option_i32 *option, int default_int)
{
    if (Option_i32_is_some(option))
    {
        return Option_i32_get(option);
    }
    else 
    {
        Option_i32_delete(option);
        return default_int;
    }
}

int Option_i32_get_or_else(Option_i32 *option, int (^default_func)())
{
    if (Option_i32_is_some(option))
    {
        return Option_i32_get(option);
    }
    else 
    {
        Option_i32_delete(option);
        return default_func();
    }
}
