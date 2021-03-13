#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "libparse.h"

#include "test_table.h"

// To avoid infinite loops.
// If tests in test_table >= 50, change this value
#define POSSIBLE_MAX_TEST_CASES (50)

#define PARAMS_VALID            (1)
#define PARAMS_INVALID          (!PARAMS_VALID)

/* This is the same data stored in myData struct (see below) */
enum {
    NUM1 = 0,
    STR1,
    STR2,
    NUM2,
    N_DATA
};

struct myData
{
    int num1;
    char * str1;
    char * str2;
    int num2;
};

// Callback function
int parseCallback(char *key, char *value, void *userData);

void print_argcv(int argc, char * argv[]);

int run_tests(int user_argc, char * user_argv[]);
int run_table_tests(void);

int run_data_tests(void);
char * is_data_numeric(char * str);
int verify_data_integrity(struct myData * data, char * original[]);

int run_user_test(int argc, char * argv[]);

int 
main(int argc, char * argv[]) 
{
    return run_tests(argc, argv);
}

/* Custom callback for testing. */
int parseCallback(char *key, char *value, void *userData)
{
    int ret_val = PARAMS_VALID;

    printf("\t\t\tparseCallback: I've been called.\n");

    // strstr return NULL when INVALID_CALLBACK_STR is not found in value
    char * is_invalid = strstr(value, INVALID_CALLBACK_STR);
    if (key)
    {
        char * is_invalid_key = strstr(key, INVALID_CALLBACK_STR);

        printf("\t\t\tparseCallback: Key: %s | ", key);
        printf(" Value: %s\n", value);

        if (is_invalid_key)
        {
            printf("\t\t\tparseCallback: Invalid key detected!\n");
            ret_val = PARAMS_INVALID;
        }
        else if (is_invalid)
        {
            printf("\t\t\tparseCallback: Invalid value detected!\n");
            ret_val = PARAMS_INVALID;
        }
        // Store nums in userData
        else if (strstr(key, "num1") != NULL && is_data_numeric(value) != NULL)
        {
            ((struct myData*) userData)->num1 = atoi(value);
        }
        else if (strstr(key, "num2") != NULL && is_data_numeric(value) != NULL)
        {
            ((struct myData*) userData)->num2 = atoi(value);
        }
    }
    else
    {
        printf("\t\t\tparseCallback: Parameter: %s\n", value);

        if (is_invalid)
        {
            printf("\t\t\tparseCallback: Invalid parameter detected!\n");
            ret_val = PARAMS_INVALID;
        }
        // Store strs in userData
        else if (strstr(value, "str1") != NULL)
        {
            ((struct myData*) userData)->str1 = value;
        }
        else if (strstr(value, "str2") != NULL)
        {
            ((struct myData*) userData)->str2 = value;
        }
    }

    return ret_val;
}

/* Print a list of argv */
void print_argcv(int argc, char * argv[])
{
    int i = 0;
    printf("\targc: %d\n", argc);
    printf("\targv:\n");
    while (i < argc)
    {
        printf("\t\targv[%d]: %s\n", i, argv[i]);
        i++;
    }
}

int run_tests(int user_argc, char * user_argv[])
{
    int failed = 0;

    printf("Beggining tests.\n");

    failed += run_table_tests();
    putchar('\n');

    failed += run_data_tests();
    putchar('\n');

    failed += run_user_test(user_argc, user_argv);

    return failed;
}

/* Test suite stored in table under get_table. Does not make use of myData */
int run_table_tests(void)
{
    int i = 0;
    int test_passed = 0, test_failed = 0;

    struct myData userData = {0};
    
    table_t table = get_table();

    i = 0;
    while (table[i] && i < POSSIBLE_MAX_TEST_CASES)
    {
        int j = 0; 

        int fargc = 0;
        char ** fargv = table[i];
        int n_parsed = 0;

        for (j = 0; fargv[j] != NULL; j++)
        {
            fargc++;
        }

        printf("=== Test %d ===\n", i+1);
        print_argcv(fargc, fargv);
        printf("\tExpected return: %s\n", fargv[fargc+1]);
        
        n_parsed = parseCmdLine(fargc, fargv, &parseCallback, &userData);

        if (n_parsed != atoi(fargv[fargc+1]))
        {
            printf("****** ERROR!! ******\n");
            printf("Received %d instead of %s\n", 
                    n_parsed, fargv[fargc+1]);
            test_failed++;
        }
        else
        {
            printf("- Pass -\n");
            test_passed++;
        }

        i++;
    }

    printf("\n=== Results ===\n\tTests passed: %d\n\tTests failed: %d\n",
            test_passed, test_failed);

    return test_failed;
}

/* Run tests in which data should be stored in myData structure. */
int run_data_tests(void)
{
    int i = 0;
    int test_passed = 0, test_failed = 0;

    table_t table = get_data_table();

    i = 0;
    while (table[i] && i < POSSIBLE_MAX_TEST_CASES)
    {
        int j = 0; 

        int n_parsed = 0;

        int expected_ret_pos = 0;

        int fargc = 0;
        char ** fargv = table[i];
    
        
        struct myData userData = {0};
        char * data_set[N_DATA] = {NULL};


        for (j = 0; fargv[j] != NULL; j++)
        {
            fargc++;
        }

        printf("=== Test %d ===\n", i+1);
        print_argcv(fargc, fargv);
        
        expected_ret_pos = fargc;
        for (j = fargc+1; fargv[j] != NULL; j++)
        {
            /* Detect str1, str2, num1 and num2 keywords and store them 
               when found. */
            if (strstr(fargv[j], "str1") != NULL)
            {
                data_set[STR1] = fargv[j];
            }
            else if (strstr(fargv[j], "str2") != NULL)
            {
                data_set[STR2] = fargv[j];
                printf("\tstr2 found.\n");
            }
            else if ( strstr(fargv[j], "num1") )
            {
                data_set[NUM1] = is_data_numeric(fargv[j]+5);
                printf("\tnum1 found; num1 = %s\n", data_set[NUM1]);
            }
            else if ( strstr(fargv[j], "num2") )
            {
                data_set[NUM2] = is_data_numeric(fargv[j]+5);
                printf("\tnum2 found; num2 = %s\n", data_set[NUM2]);
            }

            expected_ret_pos++;
        }
        expected_ret_pos += 2; // Skip the NULL

        printf("\tExpected return: %s\n", fargv[expected_ret_pos]);
        
        n_parsed = parseCmdLine(fargc, fargv, &parseCallback, &userData);

        printf("userData: \n");
        printf("\tint1: %d\n\tstr1: %s\n\tstr2: %s\n\tint2: %d\n",
                userData.num1, userData.str1, 
                userData.str2, userData.num2);

        if (n_parsed != atoi(fargv[expected_ret_pos]))
        {
            printf("****** ERROR!! ******\n");
            printf("Received %d instead of %s\n", 
                    n_parsed, fargv[expected_ret_pos]);
            test_failed++;
        }
        else
        {
            if (!verify_data_integrity(&userData, data_set))
            {
                printf("- Pass -\n");
                test_passed++;
            }
            else
            {
                printf("****** ERROR!! ******\n");
                printf("Data has been modified by the library.\n");
                test_failed++;
            }
        }

        i++;
    }

    printf("\n=== Results ===\n\tTests passed: %d\n\tTests failed: %d\n",
            test_passed, test_failed);

    return test_failed;
}

/* 
   Check if a string is only made of numbers.
   Returns a pointer to the first character of the string if it's numeric. 
   NULL otherwhise.
 */
char * is_data_numeric(char * str)
{
    if (!str) return NULL;

    int i = 0;
    char * ret_position = str;

    while((str+i) != NULL && *(str+i) != '\0' && ret_position != NULL)
    {
        if (!isdigit(*(str+i)))
        {
            ret_position = NULL;
        }
        i++;
    }
    
    return ret_position;
}

/* Verify that the given data has not been modified by the library. */
/* Returns number of invalid stored fields (0 if everything is fine) */
int verify_data_integrity(struct myData * data, char * original[])
{
    int ret_val = 0;

    // Conditions are always the same:
    //  original[..] is empty and data->.. is not empty
    //  original[..] is not empty and data->.. is different or empty

    if ((original[NUM1] == NULL && data->num1 != 0)
            || (original[NUM1] != NULL && data->num1 != atoi(original[NUM1])) )
    {
        printf("Oh no! The first integer value has changed.\n");
        printf("I got: %d , instead of %s\n", data->num1, original[NUM1]);
        ret_val++;
    }
    
    if ((original[NUM2] == NULL && data->num2 != 0)
            || (original[NUM2] != NULL && data->num2 != atoi(original[NUM2])))
    {
        printf("Oh no! The second integer value has changed.\n");
        printf("I got: %d , instead of %s\n", data->num2, original[NUM2]);
        ret_val++;
    }

    if ((original[STR1] == NULL && data->str1 != NULL)
            || (original[STR1] != NULL && strcmp(data->str1, original[STR1])))
    {
        printf("Oh no! The first string has changed.\n");
        printf("I got: %s , instead of %s\n", data->str1, original[STR1]);
        ret_val++;
    }

    if ((original[STR2] == NULL && data->str2 != NULL) 
            || (original[STR2] != NULL && strcmp(data->str2, original[STR2])))
    {
        printf("Oh no! The second string has changed.\n");
        printf("I got: %s , instead of %s\n", data->str2, original[STR2]);
        ret_val++;
    }

    return ret_val;
}

/* Test the library with argc and argv */
int run_user_test(int argc, char * argv[])
{
    if (argc == 1) return 0;

    int parseCmd_return = 0;

    struct myData userData = {0};

    printf("=== User Test ===\n");
    print_argcv(argc, argv);
    parseCmd_return = parseCmdLine (argc, argv, &parseCallback, &userData);

    printf("\n=== Returned: %d ===\n", parseCmd_return);

    if (parseCmd_return == PARSECMD_FAIL) return 1;
    return 0;
}

