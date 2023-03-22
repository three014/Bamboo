#include "option.h"

struct __Option_u32_Struct
{
    unsigned int item;
    bool some;
};

Option_u32 *Option_u32_of(void *item)
{
    Option_u32 *ret = malloc(sizeof *ret);
    if (item == NULL)
    {
        ret->item = 0;
        ret->some = false;
    }
    else 
    {
        ret->item = *(unsigned int *) item;
        ret->some = true;
    }
    return ret;
}

void Option_u32_delete(Option_u32 *option)
{
    free(option);
}

bool Option_u32_is_some(Option_u32 *option)
{
    if (option == NULL)
    {
        return false;
    }
    return option->some;
}

bool Option_u32_is_some_and(Option_u32 *option, bool (^predicate)(unsigned int))
{
    if (option == NULL || 
            Option_u32_is_some(option) == false ||
            predicate == NULL)
    {
        return false;
    }
    return predicate(option->item);
}

unsigned int Option_u32_get(Option_u32 *option)
{
    int ret = option->item;
    Option_u32_delete(option);
    return ret;
}

unsigned int Option_u32_get_or(Option_u32 *option, unsigned int default_int)
{
    if (Option_u32_is_some(option))
    {
        return Option_u32_get(option);
    }
    else 
    {
        Option_u32_delete(option);
        return default_int;
    }
}

unsigned int Option_u32_get_or_else(Option_u32 *option, unsigned int (^default_func)())
{
    if (Option_u32_is_some(option))
    {
        return Option_u32_get(option);
    }
    else 
    {
        Option_u32_delete(option);
        return default_func();
    }
}
