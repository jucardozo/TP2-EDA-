#ifndef _LIBPARSE_H
#define _LIBPARSE_H 1

/******************** DEFINITIONS ********************/
#define PARSECMD_FAIL   (-1)            // Returned by parseCmdLine

// Return values that are expected to be returned by the pCallback function
#define PARAMS_VALID    (1)             // Success
#define PARAMS_INVALID  (!PARAMS_VALID) // Failure (parseCmdLine will abort)

/******************** MACROS ********************/

/******************** STRUCTURES AND TYPEDEF ********************/
typedef int (*pCallback) (char *, char*, void *);

/******************** PROTOTYPES ********************/
int parseCmdLine (int argc, char *argv[], pCallback p, void *userData);

/*
   Check if a string is only made of numbers.
   Returns a pointer to the first character of the string if it's numeric.
   NULL otherwhise.
 */
char* is_data_numeric(char* str);
#endif /* _LIBPARSE_H */
