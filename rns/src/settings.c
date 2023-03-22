#include "settings.h"

ErrNum settings_validate_arg(int arg_count, char *arg_str, int flag_index)
{
    ErrNum status = GOOD;
    if ((flag_index >= arg_count - 1) || (arg_str[0] == '-'))
    {
        status = BAD;
    }

    return status;
}

Settings * settings_from_args(int argc, char **argv)
{
    Settings *settings = (Settings *) malloc(sizeof *settings);
    ErrNum status = GOOD;
    for (int i = 1; i < argc; i++) 
    {
        if (argv[i][0] == '-')
        {
            switch (argv[i][1])
            {
                case 'n':
                    if (settings_validate_arg(argc, argv[i + 1], i) != GOOD)
                    {
                        status = BAD;
                    }
                    else 
                    {
                        strncpy(settings->ntwk_file, argv[i + 1], MAX_CHAR_LENGTH - 1);
                        settings->ntwk_file[MAX_CHAR_LENGTH - 1] = '\0';
                        i++;
                    }
                    break;

                case 's':
                    if (settings_validate_arg(argc, argv[i + 1], i) != GOOD)
                    {
                        status = BAD;
                    }
                    else 
                    {
                        strncpy(settings->sim_file, argv[i + 1], MAX_CHAR_LENGTH - 1);
                        settings->sim_file[MAX_CHAR_LENGTH - 1] = '\0';
                        i++;
                    }
                    break;

                default:
                    status = BAD;
                    break;
            } // End switch statement
        } // End check for '-' char
        else 
        {
            status = BAD;
        }
    } // End iteration of argv

    if (status != GOOD)
    {
        free(settings);
        settings = NULL;
    }

    return settings;
} // End settings_from_args()
