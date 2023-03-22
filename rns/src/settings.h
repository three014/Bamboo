#ifndef __SETTINGS_H
#define __SETTINGS_H

#include <stdlib.h>
#include <string.h>

#define GOOD 1
#define BAD 0
typedef unsigned int ErrNum;

#define MAX_CHAR_LENGTH 256

typedef struct __Settings_Struct
{
    char ntwk_file[MAX_CHAR_LENGTH];
    char sim_file[MAX_CHAR_LENGTH];
} Settings;

ErrNum settings_validate_arg(int arg_count, char *arg_str, int flag_index);
Settings * settings_from_args(int argc, char **argv);


#endif
