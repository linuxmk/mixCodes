#ifndef BSEARCH_H
#define BSEARCH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

#define numberOfLines  1500

typedef struct range
{
    size_t keyIdexStart;
    size_t keyIndexEnd;
}range_t;


typedef struct csvFile
{
    char *key[numberOfLines];
    int numOflLines;
    range_t range;
}csvFile_t;


/**
* Parse the arguments passed from the terminal
* @param argument count
* @param array of pointers to the arguments
* @param OUT key to find
* @param OUT filename to open
* @return void
*/
void parseCmdArgs(int argc, char *argv[], char **key, char **filename);

/**
* optimisation of binary search algo for finding the left most key when there are duplicate elements
* @param filename that needs to be searched
* @param key to be search
* @return EXIT_SUCCESS if key/s are found or EXIT_FAILURE if key is not found
*/
int bSearch(const char *filename, const char *key );
#endif // BSEARCH_H
