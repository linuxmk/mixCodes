#include "fnmtch.h"

int main(int argc, char *argv[])
{
    char *pattern = NULL;
    char *filename = NULL;

    parseCmdArgs(argc, argv, &pattern, &filename);


    if(!pattern || !filename)
    {
        fprintf(stderr, "ZUsage: %s --pattern 'test*' --file inputFile.txt\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    return matchPattern(filename, pattern);
}


