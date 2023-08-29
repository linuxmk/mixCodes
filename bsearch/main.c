#include "bsearch.h"

int main(int argc, char *argv[])
{
    char *key = NULL;
    char *filename = NULL;

    parseCmdArgs(argc, argv, &key, &filename);

    if(!key || !filename)
    {
        fprintf(stderr, "Usage: %s --key keyName --file inputFile.csv\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    return bSearch( filename, key );
}
