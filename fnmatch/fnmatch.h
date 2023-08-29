#ifndef FNMTCH_H
#define FNMTCH_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fnmatch.h>
#include <getopt.h>


/**
* Parse the arguments passed from the terminal
* @param argument count
* @param array of pointers to the arguments
* @param pattern to match
* @param filename to open
* @return void
*/
void parseCmdArgs(int argc, char *argv[], char **pattern, char **filename);

/**
* Read the content of the file line by line, and trying to match the pattern
* @param filename to be open
* @param pattern to match
* @return void
*/
int matchPattern(const char *filename, const char *pattern);

#endif // FNMTCH_H
