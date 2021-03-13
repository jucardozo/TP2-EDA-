#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "libparse.h"

#define SEPARATOR       (' ')
#define KEY_INIT_CHAR   ('-')


#ifdef VERBOSE
/* Prints error messages before exiting */
static int parse_error(char * str);
#endif

int parseCmdLine (int argc, char *argv[], pCallback p, void *userData)
{
    int i = 0;
    int n_params_opts_parsed = 0;

    // Verify inputs are valid. (Not NULL)
    if(argc < 1)
    {
        printf("Invalid argc.\n");
        return PARSECMD_FAIL;
    }
    if(argv == NULL || argv[0] == NULL)
    {
        printf("Invalid argv.\n");
        return PARSECMD_FAIL;
    }
    if(p == NULL)
    {
        printf("Empty callback function.\n");
        return PARSECMD_FAIL;
    }
    if(userData == NULL)
    {
        printf("Empty userData structure.\n");
        return PARSECMD_FAIL;
    }
    
    // The arguments to parse are always (argc-1)
    for (i = 1; i < argc; i++) 
    {

#ifdef DEBUG
        printf("i: %d\n", i);
        printf("argv[%d] size: %ld\n", i, strlen(argv[i]));
        printf("argv[%d]: %s\n", i, argv[i]);
        printf("argv[%d]: %s\n", i+1, argv[i+1]);
        printf("*argv[%d][0]: %c \n", i, argv[i][0]);
#endif

        switch (argv[i][0])
        {
            case KEY_INIT_CHAR: // Key-value pair
               if (i == argc - 1 ||            // Type 1 error
                       strlen(argv[i]) == 1 ||
                       isspace(argv[i+1][0]))    // Type 2 error
                {
#ifdef VERBOSE
                   //Send appropriate error message.
                   return parse_error(argv[i]);
#else
                   return PARSECMD_FAIL;
#endif
                }

                //Try to parse, if key or value is invalid...
               if (p(argv[i] + 1, argv[i + 1], userData) == PARAMS_INVALID)
               {
#ifdef VERBOSE
                   //Send appropriate error message.
                   return parse_error(argv[i]);
#else
                   return PARSECMD_FAIL;
#endif
               }
                ++i; //Skip value, already parsed.
                ++n_params_opts_parsed; //Count successful parse.
                break;
                
            case SEPARATOR: //Ignore spaces
                break;

            default: // Parameter
                //Try to parse parameter, if invalid...
                if (p (NULL, argv[i], userData) == PARAMS_INVALID)
                {
#ifdef VERBOSE
                    //Send appropriate error message.
                    return parse_error(argv[i]);
#else
                    return PARSECMD_FAIL;
#endif
                }
                ++n_params_opts_parsed; //Count successful parse.
                break;
        }
    }

    return n_params_opts_parsed;
}


#ifdef VERBOSE
// str is always argv[i].
static int parse_error(char * str)
{
    if(!str){
	printf("NULL argument on parse_error().\n");
    	return PARSECMD_FAIL;
    }

    if (str[0] == KEY_INIT_CHAR) 
    {
        if (strlen(str) == 1)
        {
            printf("Empty key detected.\n");
        }
        else
        {
            printf("Key \'%s\' is invalid or value is missing.\n", str);
        }
    }
    else 
    {
        printf("Invalid parameter or argument: %s\n", str);
    }

    return PARSECMD_FAIL; //Failure state flag.
}
#endif

char* is_data_numeric(char* str)
{
    if (!str) return NULL;

    int i = 0;
    char* ret_position = str;

    while ((str + i) != NULL && *(str + i) != '\0' && ret_position != NULL)
    {
        if (!isdigit(*(str + i)))
        {
            ret_position = NULL;
        }
        i++;
    }

    return ret_position;
}

