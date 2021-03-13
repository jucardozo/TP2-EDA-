#include <stdlib.h>

void * get_table(void)
{
    /* All argv's */
    /* Table at the end of this function */

    /* List if argv parameters, comma separated.
     After the last argv value, it must exist a NULL and then 
     the expected return value.
     
     = { "exec_name", "argv[1]", "argv[2]", 
            "argv[3]", NULL, "expected_return"
       };
     */
    static const char * fake_argv0[] = {"./a",
        "-key", "value", NULL, 
        "1"};
    static const char * fake_argv1[] = {"./a", 
        "parameter", NULL, 
        "1"};
    static const char * fake_argv2[] = {"./a", 
        "-key", "value", "parameter", NULL, 
        "2"};
    static const char * fake_argv3[] = {"./a", 
        "parameter", "-key", "value", NULL, 
        "2"};
    static const char * fake_argv4[] = {"./a", 
        "parameter1", "parameter2", "parameter3", "-key", "value", NULL, 
        "4"};
    static const char * fake_argv5[] = {"./a", 
        "-key1", "value1", "-key2", "value2", NULL, 
        "2"};
    static const char * fake_argv6[] = {"./a", 
        "parameter1", "-key1", "value1", "-key2", "value2", "parameter2", 
        NULL, 
        "4"};
    static const char * fake_argv7[] = {"./a", 
        "parameter1", "-key1", "value1", "parameter2", "-key2", "value2", 
        "parameter3", NULL, 
        "5"};
    static const char * fake_argv8[] = {"./a", 
        "-key1", "value1", "-", "value2", "parameter1", NULL, 
        "-1"};
    static const char * fake_argv9[] = {"./a", 
        "-key1", "value1", "-key2", "value2", "-key3", NULL, 
        "-1"};
    static const char * fake_argv10[] = {"./a", 
        "-key1", "value1", "-key2", "value2", "-", NULL, 
        "-1"};
    static const char * fake_argv11[] = {"./a", 
        "-", "value1", "-key2", "value2", "-", NULL, 
        "-1"};
    static const char * fake_argv12[] = {"./a", 
        "parameter1", "-", "value1", "-key2", "value2", "-", NULL, 
        "-1"};

    static const char * fake_argv13[] = {"./a", 
        "invalid_callback_parameter", "-key", "value1", NULL, 
        "-1"
    };
    
    static const char * fake_argv14[] = {"./a", 
        "parameter", "-invalid_callback_key", "value1", NULL, 
        "-1"
    };
    
    static const char * fake_argv15[] = {"./a", 
        "parameter", "-key", "invalid_callback_value", NULL, 
        "-1"
    };

    static const void * table[] = { 
        &fake_argv0, &fake_argv1, &fake_argv2, &fake_argv3, &fake_argv4,
        &fake_argv5, &fake_argv6, &fake_argv7, &fake_argv8, &fake_argv9, 
        &fake_argv10, &fake_argv11, &fake_argv12, &fake_argv13, &fake_argv14,
        &fake_argv15,
        NULL
    };

    return table;
}

void * get_data_table(void)
{
    /* All argv's */
    /* Table at the end of this function */

    /* -num1 : store value of this key in num1
       -num2 : store value of this key in num2
        str1 : if a parameter contains this substring, is stored in str1
        str2 : if a parameter contains this substring, is stored in str2

       = {"exec_name",
       "argv", "list", NULL,
       "expected", "results", "stored", "in", "data", NULL
       "expected return (int)"
       };
    
    NOTE: if the expected return is a string (parameter), it should be 
        the same as the given in argv
        numeric results (-num1 and -num2) must be stored as "num1 1" for
        comparing with argv: '"-num1", "1"'
     */
    static const char * fake_argv0[] = {"./a",
        "-num1", "1", "str1abc", "str2", "-num2", "1230", NULL,
        "num1 1", "str1abc", "str2", "num2 1230", NULL,
        "4"
    };

    static const char * fake_argv1[] = {"./a",
        "-num1", "1", "-num2", "5467897", NULL,
        "num1 1", "num2 5467897", NULL,
        "2"
    };

    static const char * fake_argv2[] = {"./a",
        "str1", "-num1", "1", NULL,
        "str1", "num1 1", NULL,
        "2"
    };
    
    static const char * fake_argv3[] = {"./a",
        "str1", "-num1", "1", "str2", "-num2", "123invalid_callback", NULL,
        "str1", "num1 1", "str2", NULL,
        "-1"
    };

    static const void * table[] = { 
        &fake_argv0, &fake_argv1, &fake_argv2, &fake_argv3,
        NULL
    };

    return table;
}
